import ipdb

def get_cpp_type(py_type):
    if py_type == 'str':
        ret = 'std::string'
    else:
        ret = py_type
    return ret

def generate_struct_def(struct_def, out_fd):
    # struct def
    print(f"struct {struct_def.name} {{", file = out_fd)
    for m_def in struct_def.members:
        #ipdb.set_trace()
        m_cpp_type = get_cpp_type(m_def[1])
        print(f" {m_cpp_type} {m_def[0]};", file = out_fd);
    print("};", file = out_fd)

    #get_struct_descriptor
    print(f"template <> inline StructDescriptor get_struct_descriptor<{struct_def.name}>()", file = out_fd)
    print("{", file = out_fd)
    print(" static const StructDescriptor sd = {", file = out_fd)
    for m_def in struct_def.members:
        print(f"  make_member_descriptor(\"{m_def[0]}\", &{struct_def.name}::{m_def[0]}),", file = out_fd)
    print(" };", file = out_fd)
    print(" return sd;", file = out_fd)
    print("}", file = out_fd)
    
def generate_enum_def(enum_def, out_fd):
    #enum def
    print(f"enum class {enum_def.name} {{", file = out_fd);
    for m_def in enum_def.members:
        if isinstance(m_def[1], int):
            print(f"{m_def[0]} = {m_def[1]},", file = out_fd)
        else:
            print(f"{m_def[0]},")
    print("};", file = out_fd)

    # get_enum_value_string
    print(f"template <> inline std::string get_enum_value_string<{enum_def.name}>({enum_def.name} v) {{", file = out_fd)
    print(" std::string ret;", file = out_fd)
    print(" switch (v) {", file = out_fd)
    for m_def in enum_def.members:
        print(f"  case {enum_def.name}::{m_def[0]}: ret = \"{m_def[0]}\"; break;", file = out_fd)
    print("  }", file = out_fd)
    print(" return ret;", file = out_fd)
    print("}", file = out_fd)

    # set_enum_value
    print(f"template <> inline void set_enum_value<{enum_def.name}>({enum_def.name}* v, const std::string& new_v)", file = out_fd)
    print("{", file = out_fd)    
    m_def = enum_def.members[0]
    print(f" if (new_v == \"{m_def[0]}\") *v = {enum_def.name}::{m_def[0]};", file = out_fd)
    for m_def in enum_def.members[1:]:
        print(f" else if (new_v == \"{m_def[0]}\") *v = {enum_def.name}::{m_def[0]};", file = out_fd)
    print(" else {", file = out_fd)
    print("  std::ostringstream m;", file = out_fd)
    print(f"  m << \"set_enum_value for {enum_def.name}: unknown string \" << new_v;", file = out_fd)
    print("  throw runtime_error(m.str());", file = out_fd)
    print(" }", file = out_fd)
    print("}", file = out_fd)
        
    
def generate_interface_def(interface_def, out_fd):
    print(f"class {interface_def.name} : public Dipole::Object {{", file = out_fd)
    print("public:", file = out_fd)
    print(f"typedef {interface_def.name}Ptr ptr;", file = out_fd)
    #ipdb.set_trace()
    for m_def in interface_def.methods:
        m_cpp_ret_type = get_cpp_type(m_def.method_ret_type)
        #ipdb.set_trace()
        m_args = ", ".join([" ".join([get_cpp_type(x[1]), x[0]])  for x in m_def.method_args])
        print(f"virtual {m_cpp_ret_type} {m_def.method_name}({m_args}) = 0;", file = out_fd)
    print("};", file = out_fd)

    
def generate_cpp_file(module_def, out_fd):
    for enum_def in module_def.enums:
        generate_enum_def(enum_def, out_fd)
        
    for struct_def in module_def.structs:
        generate_struct_def(struct_def, out_fd)

    for interface_def in module_def.interfaces:
        generate_interface_def(interface_def, out_fd)
        

import ipdb

def get_cpp_type(py_type):
    if py_type == 'str':
        ret = 'std::string'
    elif py_type == None:
        ret = 'void'
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
        

def generate_interface_client_declarations(interface_def, out_fd):
    class_name = interface_def.name + "Ptr"
    print(f"class {class_name} {{", file = out_fd)
    print("private:", file = out_fd)
    print("  Dipole::Communicator* comm{nullptr};", file = out_fd)
    print("  std::shared_ptr<ix::WebSocket> ws;", file = out_fd)
    print("public:", file = out_fd)
    print("  std::string object_id;", file = out_fd)
    print("  std::string ws_url;", file = out_fd)
    print(f"  {class_name}();", file = out_fd)
    print(f"  {class_name}(Dipole::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);", file = out_fd)
    print(f"  {class_name}(Dipole::Communicator* comm, const std::string& object_id);", file = out_fd)
    for m_def in interface_def.methods:
        m_cpp_ret_type = get_cpp_type(m_def.method_ret_type)
        #ipdb.set_trace()
        m_args = ", ".join([" ".join([get_cpp_type(x[1]), x[0]])  for x in m_def.method_args])
        print(f"  {m_cpp_ret_type} {m_def.method_name}({m_args});", file = out_fd)
    print("};", file = out_fd)

    # get_struct_descriptor for Ptr class
    print(f"template <> inline StructDescriptor get_struct_descriptor<{class_name}>() {{", file = out_fd)
    print("{", file = out_fd)
    print(" static const StructDescriptor sd = {", file = out_fd)
    print(f"   make_member_descriptor(\"object_id\", &{class_name}::object_id),", file = out_fd)
    print(f"   make_member_descriptor(\"ws_url\", &{class_name}::ws_url),", file = out_fd)
    print(" };", file = out_fd)
    print(" return sd;", file = out_fd)
    print("}", file = out_fd)
    
def generate_interface_server_declarations(interface_def, out_fd):
    print(f"class {interface_def.name} : public Dipole::Object {{", file = out_fd)
    print("public:", file = out_fd)
    print(f" typedef {interface_def.name}Ptr ptr;", file = out_fd)
    #ipdb.set_trace()
    for m_def in interface_def.methods:
        m_cpp_ret_type = get_cpp_type(m_def.method_ret_type)
        #ipdb.set_trace()
        m_args = ", ".join([" ".join([get_cpp_type(x[1]), x[0]])  for x in m_def.method_args])
        print(f" virtual {m_cpp_ret_type} {m_def.method_name}({m_args}) = 0;", file = out_fd)
    print("};", file = out_fd)

    # method implementations
    for m_def in interface_def.methods:
        method_impl_class_name = f"{interface_def.name}__{m_def.method_name}"

        # method impl class
        print(f"struct {method_impl_class_name} : public Dipole::method_impl", file = out_fd)
        print("{", file = out_fd)
        print(" struct args_t {", file = out_fd)
        m_args = ";\n".join([" ".join([get_cpp_type(x[1]), x[0]])  for x in m_def.method_args])
        print(f" {m_args}", file = out_fd)
        print(" };", file = out_fd)
        print(" struct return_t {", file = out_fd)
        m_cpp_ret_type = get_cpp_type(m_def.method_ret_type)
        print(f"   {m_cpp_ret_type} retval;", file = out_fd)
        print(" };", file = out_fd)
        print(" void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;", file = out_fd)

        print("};", file = out_fd)

        # get_struct_descriptor for args_t, return_t, Request<args_t>, Response<return_t>
        print(f"template <> inline StructDescriptor get_struct_descriptor<{method_impl_class_name}::args_t>()", file = out_fd)
        print("{", file = out_fd)
        print(" static const StructDescriptor sd = {", file = out_fd)
        for m_arg in m_def.method_args:
            print(f"  make_member_descriptor(\"{m_arg[0]}\", &{method_impl_class_name}::args_t::{m_arg[0]}),", file = out_fd)
        print(" };", file = out_fd)
        print(" return sd;", file = out_fd)
        print("}", file = out_fd)
        
        print(f"template <> inline StructDescriptor get_struct_descriptor<Dipole:Request<{method_impl_class_name}::args_t>>()", file = out_fd)
        print("{", file = out_fd)
        print(f" return get_StructDescriptorT<{method_impl_class_name}::args_t, Dipole::Request>::get_struct_descriptor();", file = out_fd)
        print("}", file = out_fd)

        print(f"template <> inline StructDescriptor get_struct_descriptor<{method_impl_class_name}::return_t>()", file = out_fd)
        print("{", file = out_fd)
        print(" static const StructDescriptor sd = {", file = out_fd)
        print(f"  make_member_descriptor(\"ret\", &{method_impl_class_name}::return_t::retval),", file = out_fd)
        print(" };", file = out_fd)
        print(" return sd;", file = out_fd)
        print("}", file = out_fd)

        print(f"template <> inline StructDescriptor get_struct_descriptor<Dipole:Response<{method_impl_class_name}::return_t>>()", file = out_fd)
        print("{", file = out_fd)
        print(f" return get_StructDescriptorT<{method_impl_class_name}::return_t, Dipole::Request>::get_struct_descriptor();", file = out_fd)
        print("}", file = out_fd)

def generate_interface_client_definitions(interface_def, out_fd):
    class_name = f"{interface_def.name}Ptr"
    print(f"inline {class_name}::{class_name}() {{", file = out_fd)
    print("{", file = out_fd)
    print("}", file = out_fd)
    print(f"inline {class_name}::{class_name}(Dipole::Communicator* comm,", file = out_fd)
    print("std::shared_ptr<ix::WebSocket> ws,", file = out_fd)
    print("const std::string& ws_url, const std::string& object_id)", file = out_fd)
    print("{", file = out_fd)
    print(" this->comm = comm;", file = out_fd)
    print(" this->ws = ws;", file = out_fd)
    print(" this->ws_url = ws_url;", file = out_fd)
    print(" this->object_id = object_id;", file = out_fd)
    print("}", file = out_fd)
    print(f"inline {class_name}::{class_name}(Dipole::Communicator* comm, const std::string& object_id)", file = out_fd)
    print("{", file = out_fd)
    print(" this->comm = comm;", file = out_fd)
    print(" this->object_id = object_id;", file = out_fd)
    print("}", file = out_fd)

    for m_def in interface_def.methods:
        generate_interface_client_method_definition(class_name, m_def, out_fd)

def generate_interface_client_method_definition(class_name, m_def, out_fd):
    # Ptr methods
    method_impl_class_name = f"{m_def.class_def.name}__{m_def.method_name}"
    print(f"inline {m_def.method_ret_type} {class_name}::{m_def.method_name}(...args...)", file = out_fd)
    print("{", file = out_fd)
    ptr_method_template = f"""
    Dipole::Request<{method_impl_class_name}::args_t> req{{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "{method_impl_class_name}",
      .object_id = object_id,
      .args = {method_impl_class_name}::args_t()
      }};
    req.args.hello = hello;
    string ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    Dipole::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    Dipole::Response<{method_impl_class_name}::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
    """
    print(ptr_method_template, file = out_fd)
    print("}", file = out_fd)

def generate_interface_server_method_impls(interface_def, out_fd):
    for m_def in interface_def.methods:
        generate_interface_server_method_impl_definition(interface_def, m_def, out_fd)
    
def generate_interface_server_method_impl_definition(interface_def, m_def, out_fd):
    method_impl_do_call_tmpl = f"""
    Dipole::Request<args_t> req;
    from_json(&req, req_s);
    
    auto o = comm->find_object(req.object_id);
    ostringstream res_os;
    if (auto self = dynamic_pointer_cast<{interface_def.name}>(o);
	self != nullptr) {{
      try {{
	Dipole::Response<return_t> res;
	res.message_id = Dipole::create_new_message_id();
	res.orig_message_id = req.message_id;
	res.retval.retval = self->{m_def.method_name}(... args ...);
	to_json(res_os, res);
      }} catch (exception& e) {{
	Dipole::ExceptionResponse eres;
	eres.message_id = Dipole::create_new_message_id();
	eres.orig_message_id = req.message_id;
	eres.remote_exception_text = e.what();
	to_json(res_os, eres);
      }}
    }} else {{
      throw runtime_error("dyn type mismatch");
    }}

    *res_s = res_os.str();

    """

    class_name = f"{interface_def.name}__{m_def.method_name}"
    print(f"inline void {class_name}::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket>) override", file = out_fd)
    print("{", file = out_fd)
    print(method_impl_do_call_tmpl, file = out_fd)
    print("}", file = out_fd)

def generate_interface_server_method_impl_regs(interface_def, out_fd):
    for m_def in interface_def.methods:
        class_name = f"{interface_def.name}__{m_def.method_name}"
        print(f"static int _i = Dipole::RemoteMethods::register_method(\"{class_name}\", std::make_shared<{class_name}>());", file = out_fd)
        
def generate_cpp_file(module_def, out_fd):
    for enum_def in module_def.enums:
        generate_enum_def(enum_def, out_fd)
        
    for struct_def in module_def.structs:
        generate_struct_def(struct_def, out_fd)

    for interface_def in module_def.interfaces:
        generate_interface_client_declarations(interface_def, out_fd)
        
    for interface_def in module_def.interfaces:
        generate_interface_server_declarations(interface_def, out_fd)
        
    for interface_def in module_def.interfaces:
        generate_interface_client_definitions(interface_def, out_fd)

    for interface_def in module_def.interfaces:
        generate_interface_server_method_impls(interface_def, out_fd)
        
    for interface_def in module_def.interfaces:
        generate_interface_server_method_impl_regs(interface_def, out_fd)

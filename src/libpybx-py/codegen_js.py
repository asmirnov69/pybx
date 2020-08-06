import ipdb

def generate_prolog(source_pybx_fn, out_fd):
    prolog_code = f"""//
// generated code: source - {source_pybx_fn}
import * as libpybx from 'libpybx-js';
    """
    print(prolog_code, file = out_fd)

def generate_struct_def(struct_def, out_fd):
    # struct def
    print(f"export class {struct_def.name} {{", file = out_fd)
    #ipdb.set_trace()
    args = ", ".join([x.get_member_name() for x in struct_def.fields])
    print(f"  constructor({args}) {{", file = out_fd)
    for m_def in struct_def.fields:
        #ipdb.set_trace()
        m_name = m_def.get_member_name()
        print(f"    this.{m_name} = {m_name};", file = out_fd);
    print("  }", file = out_fd)
    print("};", file = out_fd)

def generate_interface_client_declarations(interface_def, out_fd):
    class_name = interface_def.name + "Ptr"
    print(f"export class {class_name} {{", file = out_fd)
    print("  constructor(o_ptr) {", file = out_fd)
    print("    this.o_ptr = o_ptr;", file = out_fd)
    for m_def in interface_def.methods:
        print(f"  this.{m_def.name} = this.{m_def.name}.bind(this);", file = out_fd)
    print("  }", file = out_fd)
    print("   to_json() { return {'object_id': this.o_ptr.object_id}; }", file = out_fd)

    # methods
    for m_def in interface_def.methods:
        args = ", ".join(m_def.get_method_args())
        m_signature = f"{interface_def.name}__{m_def.name}"
        print(f"   {m_def.name}({args}) {{", file = out_fd)
        print("    let p = new Promise((resolve, reject) => {", file = out_fd)
        print("         let call_req = {", file = out_fd)
        print("               'message-type': 'method-call',", file = out_fd)
        print("               'message-id': libpybx.generateQuickGuid(),", file = out_fd)
        print("               'object-id': this.o_ptr.object_id,", file = out_fd)
        print(f"              'method-signature': '{m_signature}',", file = out_fd)
        print("          'args': {", file = out_fd)

        for arg in m_def.get_method_args():
            arg_value = f"{arg}.to_json()"
            print(f"                 {arg}: {arg_value},", file = out_fd)
        print("          }", file = out_fd)
        print("       };", file = out_fd)

        print("	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);", file = out_fd)
        print("      this.o_ptr.ws.send(JSON.stringify(call_req));", file = out_fd)
        print("   });", file = out_fd)

        """
        # JSON return
        print(" return p.then(ret => {", file = out_fd)
        ret_type = m_def.get_method_return_type()
        if ret_type.is_fundamental_type():
            print(" let tret = ret;", file = out_fd)
        elif ret_type.is_vector(typedefs):
            value_type = ret_type.get_vector_value_type(typedefs)
            print("   let tret = [];", file = out_fd)
            print("   for (let i = 0; i < ret.lenght; i++) {", file = out_fd)
            print(f"    let o = new {value_type}(); Object.assign(o, ret[i]);", file = out_fd)
            print("     tret.push(o);", file = out_fd)
            print("   }", file = out_fd)
        else:
            print(f" let tret = new {ret_type.py_type}(); Object.assign(tret, ret);", file = out_fd)

        print("  return tret; });", file = out_fd)
        print("  }", file = out_fd)
        """
    print("};", file = out_fd)

def generate_interface_server_declarations(interface_def, out_fd):
    print(f"export class {interface_def.name}", file = out_fd)
    print("{", file = out_fd)
    print("   __call_method(method, args) {", file = out_fd)
    print("      method = method.split(\"__\")[1];", file = out_fd)
    for m_def in interface_def.methods:
        print(f"   if (method == '{m_def.name}') {{", file = out_fd)
        print(f"   //return this.{m_def.name}(...args);", file = out_fd)
        print(f"   return this.{m_def.name}(Array.from(args));", file = out_fd)
        print(f"}}", file = out_fd)
    print("  throw new Error(\"unknown method \" + method)", file = out_fd)
    print("}", file = out_fd)

    for m_def in interface_def.methods:
        args = ", ".join(m_def.get_method_args())
        print(f"{m_def.name}({args}) {{throw new Error(\"not implemented\");}}", file = out_fd)
    print("};", file = out_fd)
    
def generate_js_file(module_def, out_fd, source_pybx_fn):
    generate_prolog(source_pybx_fn, out_fd)

    #for enum_def in module_def.enums:
    #    generate_enum_def(enum_def, out_fd)
        
    for struct_def in module_def.structs:
        generate_struct_def(struct_def, out_fd)

    #generate_interface_client_forward_declarations(module_def, out_fd)
    #generate_typedef_declarations(module_def, out_fd)
    
    for interface_def in module_def.interfaces:
        generate_interface_client_declarations(interface_def, out_fd)
            
    for interface_def in module_def.interfaces:
        generate_interface_server_declarations(interface_def, out_fd)

    """
    #for interface_def in module_def.interfaces:
    #    generate_interface_client_definitions(interface_def, out_fd)

    #for interface_def in module_def.interfaces:
    #    generate_interface_server_method_impls(module_def, interface_def, out_fd)

    #generate_epilog(out_fd)
    """

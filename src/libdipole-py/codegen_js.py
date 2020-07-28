import ipdb

def generate_prolog(source_pyidl_fn, out_fd):
    prolog_code = f"""//
// generated code: source - {source_pyidl_fn}
import * as libdipole from 'libdipole-js';
    """
    print(prolog_code, file = out_fd)

def generate_struct_def(struct_def, out_fd):
    # struct def
    print(f"export class {struct_def.name} {{", file = out_fd)
    #ipdb.set_trace()
    args = ", ".join([x.name for x in struct_def.members])
    print(f"  constructor({args}) {{", file = out_fd)
    for m_def in struct_def.members:
        #ipdb.set_trace()
        print(f"    this.{m_def.name} = {m_def.name};", file = out_fd);
    print("  }", file = out_fd)
    print("};", file = out_fd)

def generate_interface_client_declarations(interface_def, typedefs, out_fd):
    class_name = interface_def.name + "Ptr"
    print(f"export class {class_name} {{", file = out_fd)
    print("  constructor(o_ptr) {", file = out_fd)
    print("    this.o_ptr = o_ptr;", file = out_fd)
    for m_def in interface_def.methods:
        print(f"  this.{m_def.method_name} = this.{m_def.method_name}.bind(this);", file = out_fd)
    print("  }", file = out_fd)
    print("   to_json() { return {'object_id': this.o_ptr.object_id}; }", file = out_fd)

    # methods
    for m_def in interface_def.methods:
        args = ", ".join([x.arg_name for x in m_def.method_args.args])
        m_signature = f"{interface_def.name}__{m_def.method_name}"
        print(f"   {m_def.method_name}({args}) {{", file = out_fd)
        print("    let p = new Promise((resolve, reject) => {", file = out_fd)
        print("         let call_req = {", file = out_fd)
        print("               'message-type': 'method-call',", file = out_fd)
        print("               'message-id': libdipole.generateQuickGuid(),", file = out_fd)
        print("               'object-id': this.o_ptr.object_id,", file = out_fd)
        print(f"              'method-signature': '{m_signature}',", file = out_fd)
        print("          'args': {", file = out_fd)
        for arg in m_def.method_args.args:
            arg_value = f"{arg.arg_name}.to_json()" if arg.arg_type.is_ptr_type(typedefs) else f"{arg.arg_name}"
            print(f"                 {arg.arg_name}: {arg_value},", file = out_fd)
        print("          }", file = out_fd)
        print("       };", file = out_fd)

        print("	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);", file = out_fd)
        print("      this.o_ptr.ws.send(JSON.stringify(call_req));", file = out_fd)
        print("   });", file = out_fd)

        # JSON return
        print(" return p.then(ret => {", file = out_fd)
        ret_type = m_def.method_ret_type
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

    print("};", file = out_fd)

def generate_interface_server_declarations(interface_def, out_fd):
    print(f"export class {interface_def.name}", file = out_fd)
    print("{", file = out_fd)
    print("   __call_method(method, args) {", file = out_fd)
    print("      method = method.split(\"__\")[1];", file = out_fd)
    for m_def in interface_def.methods:
        print(f"   if (method == '{m_def.method_name}') {{", file = out_fd)
        print(f"   //return this.{m_def.method_name}(...args);", file = out_fd)
        print(f"   return this.{m_def.method_name}(Array.from(args));", file = out_fd)
        print(f"}}", file = out_fd)
    print("  throw new Error(\"unknown method \" + method)", file = out_fd)
    print("}", file = out_fd)

    for m_def in interface_def.methods:
        args = ", ".join([x.arg_name for x in m_def.method_args.args])
        print(f"{m_def.method_name}({args}) {{throw new Error(\"not implemented\");}}", file = out_fd)
    print("};", file = out_fd)
    
def generate_js_file(source_pyidl_fn, module_def, out_fd):
    generate_prolog(source_pyidl_fn, out_fd)

    #for enum_def in module_def.enums:
    #    generate_enum_def(enum_def, out_fd)
        
    for struct_def in module_def.structs:
        generate_struct_def(struct_def, out_fd)

    #generate_interface_client_forward_declarations(module_def, out_fd)
    #generate_typedef_declarations(module_def, out_fd)
    
    for interface_def in module_def.interfaces:
        generate_interface_client_declarations(interface_def, module_def.typedefs, out_fd)
            
    for interface_def in module_def.interfaces:
        generate_interface_server_declarations(interface_def, out_fd)

    """
    #for interface_def in module_def.interfaces:
    #    generate_interface_client_definitions(interface_def, out_fd)

    #for interface_def in module_def.interfaces:
    #    generate_interface_server_method_impls(module_def, interface_def, out_fd)

    #generate_epilog(out_fd)
    """

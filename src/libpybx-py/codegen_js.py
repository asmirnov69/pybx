import ipdb

def generate_prolog(source_pybx_fn, out_fd):
    prolog_code = f"""//
// generated code: source - {source_pybx_fn}
import * as libpybx from 'libpybx-js';
    """
    print(prolog_code, file = out_fd)

def generate_struct_def(struct_def, out_fd):
    # struct def
    print(f"export class {struct_def.name} extends libpybx.dataclass {{", file = out_fd)
    #ipdb.set_trace()
    args = ", ".join([x.get_member_name() for x in struct_def.fields])
    print(f"  constructor({args}) {{", file = out_fd)
    print("  super();", file = out_fd)
    for m_def in struct_def.fields:
        #ipdb.set_trace()
        m_name = m_def.get_member_name()
        if m_def.get_member_type().is_struct():
            m_type = m_def.get_member_type().get_js_code_name()
            print(f"    this.{m_name} = {m_name} === undefined ? {m_type} : {m_name};", file = out_fd);
        else:
            print(f"    this.{m_name} = {m_name};", file = out_fd);
    print("  }", file = out_fd)
    print("};", file = out_fd)

def generate_interface_client_declarations(interface_def, out_fd):
    class_name = interface_def.name + "Ptr"
    print(f"export class {class_name} extends libpybx.ObjectPtr {{", file = out_fd)
    print(f"  get_type_name() {{ return '{interface_def.def_type.__module__}.{interface_def.name}'; }}", file = out_fd)
    print("  constructor(comm, ws, object_id) {", file = out_fd)
    print("     super(comm, ws, object_id);", file = out_fd)
    for m_def in interface_def.methods:
        print(f"  this.{m_def.name} = this.{m_def.name}.bind(this);", file = out_fd)
    print("  }", file = out_fd)
    print("   to_json() { return {'object_id': this.object_id}; }", file = out_fd)

    # methods
    for m_def in interface_def.methods:
        args = ", ".join(m_def.get_method_args())
        m_signature = f"{interface_def.name}__{m_def.name}"
        print(f"   {m_def.name}({args}) {{", file = out_fd)
        print("    let p = new Promise((resolve, reject) => {", file = out_fd)
        print("         let call_req = {", file = out_fd)
        print("               'message-type': 'method-call',", file = out_fd)
        print("               'message-id': libpybx.generateQuickGuid(),", file = out_fd)
        print("               'object-id': this.object_id,", file = out_fd)
        print(f"              'method-signature': '{m_signature}',", file = out_fd)
        print("          'args': {", file = out_fd)

        for arg in m_def.get_method_args():
            print(f"                 {arg}: {arg},", file = out_fd)
        print("          }", file = out_fd)
        print("       };", file = out_fd)

        print("	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);", file = out_fd)
        print(f"      console.log(\"{m_def.name}:\", libpybx.to_json_string(call_req));", file = out_fd)
        print("      this.ws.send(libpybx.to_json_string(call_req));", file = out_fd)
        print("   });", file = out_fd)
        
        # JSON return
        print(" return p.then(ret_json => {", file = out_fd)
        ret_type = m_def.get_method_return_type().get_js_code_name()
        print(f" let ret = libpybx.from_json(ret_json, {ret_type});", file = out_fd)
        print(" return ret; });", file = out_fd)
        print(" }", file = out_fd)

    print("};", file = out_fd)

def generate_interface_server_declarations(interface_def, out_fd):
    print(f"export class {interface_def.name}", file = out_fd)
    print("{", file = out_fd)
    print(f"  get_ptr_type() {{ return {interface_def.name}Ptr; }}", file = out_fd)
    print("   __call_method(method, args) {", file = out_fd)
    print("      method = method.split(\"__\")[1];", file = out_fd)
    for m_def in interface_def.methods:
        print(f"   if (method == '{m_def.name}') {{", file = out_fd)
        call_args_l = []
        arg_i = 0;
        for m_arg, m_arg_type in zip(m_def.get_method_args(), m_def.get_method_arg_types()):
            m_arg_type_tmpl = m_arg_type.get_js_code_name()
            print(f"    let arg_{arg_i} = libpybx.from_json(args.{m_arg}, {m_arg_type_tmpl});", file = out_fd)
            call_args_l.append(f"arg_{arg_i}")
            arg_i += 1
        call_args = ",".join(call_args_l)
        print(f"    return this.{m_def.name}({call_args});", file = out_fd)
        print(f"  }}", file = out_fd)
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
        
    for struct_name in module_def.struct_order:
        struct_def = module_def.structs[struct_name]
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

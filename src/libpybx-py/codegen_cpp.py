import ipdb
import os.path

def generate_prolog(source_pybx_fn, out_fd):
    macro_lock_s = os.path.basename(source_pybx_fn).split(".")[0].upper()
    prolog_code = f"""// -*- c++ -*-
// generated code: source - {source_pybx_fn}
#ifndef __{macro_lock_s}_STUBS_HH__
#define __{macro_lock_s}_STUBS_HH__

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE static int TOKENPASTE2(Unique_, __LINE__)

#include <memory>
#include <string>
#include <stdexcept>
using namespace std;

#include <kvan/json-io.h>
#include <libpybx-cpp/communicator.h>
#include <libpybx-cpp/proto.h>
#include <libpybx-cpp/remote-methods.h>
    """
    print(prolog_code, file = out_fd)

def generate_epilog(out_fd):
    print("#endif", file = out_fd)
    
def generate_struct_def(struct_def, out_fd):
    # struct def
    print(f"struct {struct_def.name} {{", file = out_fd)
    for m_def in struct_def.fields:
        #ipdb.set_trace()
        m_cpp_name = m_def.get_member_name()
        m_cpp_type = m_def.get_member_type()
        print(f" {m_cpp_type} {m_cpp_name};", file = out_fd);
    print("};", file = out_fd)

    #get_struct_descriptor
    print(f"template <> inline StructDescriptor get_struct_descriptor<{struct_def.name}>()", file = out_fd)
    print("{", file = out_fd)
    print(" static const StructDescriptor sd = {", file = out_fd)
    for m_def in struct_def.fields:
        m_cpp_name = m_def.get_member_name()
        print(f"  make_member_descriptor(\"{m_cpp_name}\", &{struct_def.name}::{m_cpp_name}),", file = out_fd)
    print(" };", file = out_fd)
    print(" return sd;", file = out_fd)
    print("}", file = out_fd)
    
def generate_enum_def(enum_def, out_fd):
    #enum def
    #ipdb.set_trace()
    print(f"enum class {enum_def.name} {{", file = out_fd);
    for m_name, m_value in zip(enum_def.members, enum_def.member_values):
        print(f"{m_name} = {m_value},", file = out_fd)
    print("};", file = out_fd)

    # get_enum_value_string
    print(f"template <> inline std::string get_enum_value_string<{enum_def.name}>({enum_def.name} v) {{", file = out_fd)
    print(" std::string ret;", file = out_fd)
    print(" switch (v) {", file = out_fd)
    for m_name, m_value in zip(enum_def.members, enum_def.member_values):
        print(f"  case {enum_def.name}::{m_name}: ret = \"{m_name}\"; break;", file = out_fd)
    print("  }", file = out_fd)
    print(" return ret;", file = out_fd)
    print("}", file = out_fd)

    # set_enum_value
    print(f"template <> inline void set_enum_value<{enum_def.name}>({enum_def.name}* v, const std::string& new_v)", file = out_fd)
    print("{", file = out_fd)
    m_name = enum_def.members[0]
    print(f" if (new_v == \"{m_name}\") *v = {enum_def.name}::{m_name};", file = out_fd)
    for m_name in enum_def.members[1:]:
        print(f" else if (new_v == \"{m_name}\") *v = {enum_def.name}::{m_name};", file = out_fd)
    print(" else {", file = out_fd)
    print("  std::ostringstream m;", file = out_fd)
    print(f"  m << \"set_enum_value for {enum_def.name}: unknown string \" << new_v;", file = out_fd)
    print("  throw runtime_error(m.str());", file = out_fd)
    print(" }", file = out_fd)
    print("}", file = out_fd)
        
def generate_interface_client_forward_declarations(module_def, out_fd):
    for interface_def in module_def.interfaces:
        print(f"class {interface_def.name}Ptr;", file = out_fd)

def generate_typedef_declarations(module_def, out_fd):
    #ipdb.set_trace()
    for typedef in module_def.typedefs:
        if typedef.typedef_container == 'List':
            print(f"typedef vector<{typedef.typedef_element_type}> {typedef.name};", file = out_fd)
        
def generate_interface_client_declarations(interface_def, out_fd):
    class_name = interface_def.name + "Ptr"
    print(f"class {class_name} {{", file = out_fd)
    print("private:", file = out_fd)
    print("  pybx::Communicator* comm{nullptr};", file = out_fd)
    print("  std::shared_ptr<ix::WebSocket> ws;", file = out_fd)
    print("public:", file = out_fd)
    print("  std::string object_id;", file = out_fd)
    print("  std::string ws_url;", file = out_fd)
    print(f"  {class_name}();", file = out_fd)
    print(f"  {class_name}(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);", file = out_fd)
    print(f"  {class_name}(pybx::Communicator* comm, const std::string& object_id);", file = out_fd)
    print(f"  void activate(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);", file = out_fd)
    for m_def in interface_def.methods:
        m_cpp_ret_type = m_def.get_method_return_type()
        m_args = ", ".join(m_def.get_method_typed_args())
        print(f"  {m_cpp_ret_type} {m_def.name}({m_args});", file = out_fd)
    print("};", file = out_fd)

    # get_struct_descriptor for Ptr class
    print(f"template <> inline StructDescriptor get_struct_descriptor<{class_name}>()", file = out_fd)
    print("{", file = out_fd)
    print(" static const StructDescriptor sd = {", file = out_fd)
    print(f"   make_member_descriptor(\"object_id\", &{class_name}::object_id),", file = out_fd)
    print(f"   make_member_descriptor(\"ws_url\", &{class_name}::ws_url),", file = out_fd)
    print(" };", file = out_fd)
    print(" return sd;", file = out_fd)
    print("}", file = out_fd)
    
def generate_interface_server_declarations(interface_def, out_fd):
    print(f"class {interface_def.name} : public pybx::Object {{", file = out_fd)
    print("public:", file = out_fd)
    print(f" typedef {interface_def.name}Ptr ptr;", file = out_fd)
    #ipdb.set_trace()
    for m_def in interface_def.methods:
        m_cpp_ret_type = m_def.get_method_return_type()
        m_args = ", ".join(m_def.get_method_typed_args())
        print(f" virtual {m_cpp_ret_type} {m_def.name}({m_args}) = 0;", file = out_fd)
    print("};", file = out_fd)

    # method implementations
    for m_def in interface_def.methods:
        method_impl_class_name = f"{interface_def.name}__{m_def.name}"

        # method impl class
        print(f"struct {method_impl_class_name} : public pybx::method_impl", file = out_fd)
        print("{", file = out_fd)
        print(" struct args_t {", file = out_fd)
        m_args = ";\n".join(m_def.get_method_typed_args()) + ";"
        print(f" {m_args}", file = out_fd)
        print(" };", file = out_fd)
        print(" struct return_t {", file = out_fd)
        m_cpp_ret_type = m_def.get_method_return_type()
        print(f"   {m_cpp_ret_type} retval;", file = out_fd)
        print(" };", file = out_fd)
        print(" void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;", file = out_fd)

        print("};", file = out_fd)

        # get_struct_descriptor for args_t, return_t, Request<args_t>, Response<return_t>
        print(f"template <> inline StructDescriptor get_struct_descriptor<{method_impl_class_name}::args_t>()", file = out_fd)
        print("{", file = out_fd)
        print(" static const StructDescriptor sd = {", file = out_fd)
        for m_arg in m_def.get_method_args():
            print(f"  make_member_descriptor(\"{m_arg}\", &{method_impl_class_name}::args_t::{m_arg}),", file = out_fd)
        print(" };", file = out_fd)
        print(" return sd;", file = out_fd)
        print("}", file = out_fd)
        
        print(f"template <> inline StructDescriptor get_struct_descriptor<pybx::Request<{method_impl_class_name}::args_t>>()", file = out_fd)
        print("{", file = out_fd)
        print(f" return get_StructDescriptor_T<{method_impl_class_name}::args_t, pybx::Request>::get_struct_descriptor();", file = out_fd)
        print("}", file = out_fd)

        print(f"template <> inline StructDescriptor get_struct_descriptor<{method_impl_class_name}::return_t>()", file = out_fd)
        print("{", file = out_fd)
        print(" static const StructDescriptor sd = {", file = out_fd)
        print(f"  make_member_descriptor(\"ret\", &{method_impl_class_name}::return_t::retval),", file = out_fd)
        print(" };", file = out_fd)
        print(" return sd;", file = out_fd)
        print("}", file = out_fd)

        print(f"template <> inline StructDescriptor get_struct_descriptor<pybx::Response<{method_impl_class_name}::return_t>>()", file = out_fd)
        print("{", file = out_fd)
        print(f" return get_StructDescriptor_T<{method_impl_class_name}::return_t, pybx::Response>::get_struct_descriptor();", file = out_fd)
        print("}", file = out_fd)

def generate_interface_client_definitions(interface_def, out_fd):
    class_name = f"{interface_def.name}Ptr"
    print(f"inline {class_name}::{class_name}()", file = out_fd)
    print("{", file = out_fd)
    print("}", file = out_fd)
    print(f"inline {class_name}::{class_name}(pybx::Communicator* comm,", file = out_fd)
    print("std::shared_ptr<ix::WebSocket> ws,", file = out_fd)
    print("const std::string& ws_url, const std::string& object_id)", file = out_fd)
    print("{", file = out_fd)
    print(" this->comm = comm;", file = out_fd)
    print(" this->ws = ws;", file = out_fd)
    print(" this->ws_url = ws_url;", file = out_fd)
    print(" this->object_id = object_id;", file = out_fd)
    print("}", file = out_fd)
    print(f"inline {class_name}::{class_name}(pybx::Communicator* comm, const std::string& object_id)", file = out_fd)
    print("{", file = out_fd)
    print(" this->comm = comm;", file = out_fd)
    print(" this->object_id = object_id;", file = out_fd)
    print("}", file = out_fd)
    print(f"inline void {class_name}::activate(pybx::Communicator* c, std::shared_ptr<ix::WebSocket> ws)", file = out_fd)

    method_activate_code = f"""
    {{
      this->comm = c;
      if (this->ws == nullptr) {{
        if (ws_url == "") {{
          this->ws = ws;
        }} else {{
          throw runtime_error("{class_name}::activate: not implemented for universal ptr");
        }}
      }}
    }}
    """
    print(method_activate_code, file = out_fd)

    for m_def in interface_def.methods:
        generate_interface_client_method_definition(class_name, m_def, out_fd)

def generate_interface_client_method_definition(class_name, m_def, out_fd):
    # Ptr methods
    method_impl_class_name = f"{class_name}__{m_def.name}"
    m_args = ", ".join(m_def.get_method_typed_args())
    m_ret_type = m_def.get_method_return_type()
    req_args_assignments = ";\n".join(["req.args." + x + "=" + x for x in m_def.get_method_args()])
    print(f"inline {m_ret_type} {class_name}::{m_def.name}({m_args})", file = out_fd)
    print("{", file = out_fd)
    ptr_method_template = f"""
    pybx::Request<{method_impl_class_name}::args_t> req{{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "{method_impl_class_name}",
      .object_id = object_id,
      .args = {method_impl_class_name}::args_t()
      }};

    {req_args_assignments};
    {m_ret_type} ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<{method_impl_class_name}::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
    """
    print(ptr_method_template, file = out_fd)
    print("}", file = out_fd)

def generate_interface_server_method_impls(module_def, interface_def, out_fd):
    for m_def in interface_def.methods:
        generate_interface_server_method_impl_definition(module_def, interface_def, m_def, out_fd)
    
def generate_interface_server_method_impl_definition(module_def, interface_def, m_def, out_fd):
    #ipdb.set_trace()
    # activations code below is temp setup capabale to handle
    # use-case of server callbacks only. More reigirous type support required
    # to support all possible combinations like vector of ptrs, returns of ptr
    # etc
    activations = []
    if 0:
        args_with_activation = []
        for arg in m_def.get_method_args():
            arg_typedef = module_def.find_typedef(arg.arg_type.py_type)
            if arg_typedef != None:
                if arg_typedef.typedef_container == 'ObjectPtr':
                    args_with_activation.append(arg)
        for arg in args_with_activation:
            activations.append(f"req.args.{arg.arg_name}.activate(comm, ws);")
    activations_code = "\n".join(activations)

    m_args = ", ".join(["req.args." + x for x in m_def.get_method_args()])
    method_impl_do_call_tmpl = f"""
    ostringstream res_os;
    try {{
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      {activations_code}

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<{interface_def.name}>(o);
      if (self == nullptr) {{
        throw runtime_error("dyn type mismatch");
      }}

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
      res.retval.retval = self->{m_def.name}({m_args});
      to_json(res_os, res);
    }} catch (exception& e) {{
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }}

    *res_s = res_os.str();
    """

    class_name = f"{interface_def.name}__{m_def.name}"
    print(f"inline void {class_name}::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)", file = out_fd)
    print("{", file = out_fd)
    print(method_impl_do_call_tmpl, file = out_fd)
    print("}", file = out_fd)
    print(f"UNIQUE = pybx::RemoteMethods::register_method(\"{class_name}\", std::make_shared<{class_name}>());", file = out_fd)

def generate_cpp_file(module_def, out_fd, source_pybx_fn):
    generate_prolog(source_pybx_fn, out_fd)

    for enum_def in module_def.enums:
        generate_enum_def(enum_def, out_fd)
        
    for struct_def in module_def.structs:
        generate_struct_def(struct_def, out_fd)

    generate_interface_client_forward_declarations(module_def, out_fd)
    #generate_typedef_declarations(module_def, out_fd)
    
    for interface_def in module_def.interfaces:
        generate_interface_client_declarations(interface_def, out_fd)

    #ipdb.set_trace()        
    for interface_def in module_def.interfaces:
        generate_interface_server_declarations(interface_def, out_fd)
        
    for interface_def in module_def.interfaces:
        generate_interface_client_definitions(interface_def, out_fd)

    for interface_def in module_def.interfaces:
        generate_interface_server_method_impls(module_def, interface_def, out_fd)

    generate_epilog(out_fd)

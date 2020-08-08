// -*- c++ -*-
// generated code: source - ./backend.pybx
#ifndef __BACKEND_STUBS_HH__
#define __BACKEND_STUBS_HH__

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
    
namespace backend {
enum class Color {
NORMAL = 0,
RED = 1,
GREEN = 2,
};
}
template <> inline std::string get_enum_value_string<backend::Color>(backend::Color v) {
 std::string ret;
 switch (v) {
  case backend::Color::NORMAL: ret = "NORMAL"; break;
  case backend::Color::RED: ret = "RED"; break;
  case backend::Color::GREEN: ret = "GREEN"; break;
  }
 return ret;
}
template <> inline void set_enum_value<backend::Color>(backend::Color* v, const std::string& new_v)
{
 if (new_v == "NORMAL") *v = backend::Color::NORMAL;
 else if (new_v == "RED") *v = backend::Color::RED;
 else if (new_v == "GREEN") *v = backend::Color::GREEN;
 else {
  std::ostringstream m;
  m << "set_enum_value for Color: unknown string " << new_v;
  throw runtime_error(m.str());
 }
}
namespace backend {
struct Greetings {
 string language;
 string text;
 backend::Color color;
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::Greetings>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("language", &backend::Greetings::language),
  make_member_descriptor("text", &backend::Greetings::text),
  make_member_descriptor("color", &backend::Greetings::color),
 };
 return sd;
}
namespace backend {
class HelloPtr;
class HelloCBPtr;
}
namespace backend {
class HelloPtr {
private:
  pybx::Communicator* comm{nullptr};
  std::shared_ptr<ix::WebSocket> ws;
public:
  std::string object_id;
  std::string __interface_type{"backend.HelloPtr"};
  HelloPtr();
  HelloPtr(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);
  HelloPtr(pybx::Communicator* comm, const std::string& object_id);
  void activate(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);
  vector<backend::Greetings> reformatGreetings(vector<backend::Greetings> gs);
  string register_hello_cb(backend::HelloCBPtr cb);
  backend::Greetings sayHello(string weSay);
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::HelloPtr>()
{
 static const StructDescriptor sd = {
   make_member_descriptor("object_id", &backend::HelloPtr::object_id),
   make_member_descriptor("__interface_type", &backend::HelloPtr::__interface_type),
 };
 return sd;
}
namespace backend {
class HelloCBPtr {
private:
  pybx::Communicator* comm{nullptr};
  std::shared_ptr<ix::WebSocket> ws;
public:
  std::string object_id;
  std::string __interface_type{"backend.HelloCBPtr"};
  HelloCBPtr();
  HelloCBPtr(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);
  HelloCBPtr(pybx::Communicator* comm, const std::string& object_id);
  void activate(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);
  string confirmHello(string hello);
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::HelloCBPtr>()
{
 static const StructDescriptor sd = {
   make_member_descriptor("object_id", &backend::HelloCBPtr::object_id),
   make_member_descriptor("__interface_type", &backend::HelloCBPtr::__interface_type),
 };
 return sd;
}
namespace backend {
class Hello : public pybx::Object {
public:
 typedef HelloPtr ptr;
 virtual vector<backend::Greetings> reformatGreetings(vector<backend::Greetings> gs) = 0;
 virtual string register_hello_cb(backend::HelloCBPtr cb) = 0;
 virtual backend::Greetings sayHello(string weSay) = 0;
};
}
namespace backend {
struct Hello__reformatGreetings : public pybx::method_impl
{
 struct args_t {
 vector<backend::Greetings> gs;
 };
 struct return_t {
   vector<backend::Greetings> retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::Hello__reformatGreetings::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("gs", &backend::Hello__reformatGreetings::args_t::gs),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<backend::Hello__reformatGreetings::args_t>>()
{
 return get_StructDescriptor_T<backend::Hello__reformatGreetings::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<backend::Hello__reformatGreetings::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &backend::Hello__reformatGreetings::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<backend::Hello__reformatGreetings::return_t>>()
{
 return get_StructDescriptor_T<backend::Hello__reformatGreetings::return_t, pybx::Response>::get_struct_descriptor();
}
namespace backend {
struct Hello__register_hello_cb : public pybx::method_impl
{
 struct args_t {
 backend::HelloCBPtr cb;
 };
 struct return_t {
   string retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::Hello__register_hello_cb::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("cb", &backend::Hello__register_hello_cb::args_t::cb),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<backend::Hello__register_hello_cb::args_t>>()
{
 return get_StructDescriptor_T<backend::Hello__register_hello_cb::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<backend::Hello__register_hello_cb::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &backend::Hello__register_hello_cb::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<backend::Hello__register_hello_cb::return_t>>()
{
 return get_StructDescriptor_T<backend::Hello__register_hello_cb::return_t, pybx::Response>::get_struct_descriptor();
}
namespace backend {
struct Hello__sayHello : public pybx::method_impl
{
 struct args_t {
 string weSay;
 };
 struct return_t {
   backend::Greetings retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::Hello__sayHello::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("weSay", &backend::Hello__sayHello::args_t::weSay),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<backend::Hello__sayHello::args_t>>()
{
 return get_StructDescriptor_T<backend::Hello__sayHello::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<backend::Hello__sayHello::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &backend::Hello__sayHello::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<backend::Hello__sayHello::return_t>>()
{
 return get_StructDescriptor_T<backend::Hello__sayHello::return_t, pybx::Response>::get_struct_descriptor();
}
namespace backend {
class HelloCB : public pybx::Object {
public:
 typedef HelloCBPtr ptr;
 virtual string confirmHello(string hello) = 0;
};
}
namespace backend {
struct HelloCB__confirmHello : public pybx::method_impl
{
 struct args_t {
 string hello;
 };
 struct return_t {
   string retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<backend::HelloCB__confirmHello::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("hello", &backend::HelloCB__confirmHello::args_t::hello),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<backend::HelloCB__confirmHello::args_t>>()
{
 return get_StructDescriptor_T<backend::HelloCB__confirmHello::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<backend::HelloCB__confirmHello::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &backend::HelloCB__confirmHello::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<backend::HelloCB__confirmHello::return_t>>()
{
 return get_StructDescriptor_T<backend::HelloCB__confirmHello::return_t, pybx::Response>::get_struct_descriptor();
}
namespace backend {
inline HelloPtr::HelloPtr()
{
}
inline HelloPtr::HelloPtr(pybx::Communicator* comm,
std::shared_ptr<ix::WebSocket> ws,
const std::string& ws_url, const std::string& object_id)
{
 this->comm = comm;
 this->ws = ws;
 this->object_id = object_id;
}
inline HelloPtr::HelloPtr(pybx::Communicator* comm, const std::string& object_id)
{
 this->comm = comm;
 this->object_id = object_id;
}
inline void HelloPtr::activate(pybx::Communicator* c, std::shared_ptr<ix::WebSocket> ws)

    {
      this->comm = c;
      if (this->ws == nullptr) {
        this->ws = ws;
      } else {
          throw runtime_error("HelloPtr::activate: not implemented for universal ptr");
      }
    }
    
inline vector<backend::Greetings> HelloPtr::reformatGreetings(vector<backend::Greetings> gs)
{

    pybx::Request<Hello__reformatGreetings::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "Hello__reformatGreetings",
      .object_id = object_id,
      .args = Hello__reformatGreetings::args_t()
      };

    req.args.gs=gs;
     vector<backend::Greetings> ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<Hello__reformatGreetings::return_t> res;
    from_json(&res, res_s.second);
     ret = res.retval.retval;
     return ret;
    
}
inline string HelloPtr::register_hello_cb(backend::HelloCBPtr cb)
{

    pybx::Request<Hello__register_hello_cb::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "Hello__register_hello_cb",
      .object_id = object_id,
      .args = Hello__register_hello_cb::args_t()
      };

    req.args.cb=cb;
     string ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<Hello__register_hello_cb::return_t> res;
    from_json(&res, res_s.second);
     ret = res.retval.retval;
     return ret;
    
}
inline backend::Greetings HelloPtr::sayHello(string weSay)
{

    pybx::Request<Hello__sayHello::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "Hello__sayHello",
      .object_id = object_id,
      .args = Hello__sayHello::args_t()
      };

    req.args.weSay=weSay;
     backend::Greetings ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<Hello__sayHello::return_t> res;
    from_json(&res, res_s.second);
     ret = res.retval.retval;
     return ret;
    
}
} // end of namespace
namespace backend {
inline HelloCBPtr::HelloCBPtr()
{
}
inline HelloCBPtr::HelloCBPtr(pybx::Communicator* comm,
std::shared_ptr<ix::WebSocket> ws,
const std::string& ws_url, const std::string& object_id)
{
 this->comm = comm;
 this->ws = ws;
 this->object_id = object_id;
}
inline HelloCBPtr::HelloCBPtr(pybx::Communicator* comm, const std::string& object_id)
{
 this->comm = comm;
 this->object_id = object_id;
}
inline void HelloCBPtr::activate(pybx::Communicator* c, std::shared_ptr<ix::WebSocket> ws)

    {
      this->comm = c;
      if (this->ws == nullptr) {
        this->ws = ws;
      } else {
          throw runtime_error("HelloCBPtr::activate: not implemented for universal ptr");
      }
    }
    
inline string HelloCBPtr::confirmHello(string hello)
{

    pybx::Request<HelloCB__confirmHello::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "HelloCB__confirmHello",
      .object_id = object_id,
      .args = HelloCB__confirmHello::args_t()
      };

    req.args.hello=hello;
     string ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<HelloCB__confirmHello::return_t> res;
    from_json(&res, res_s.second);
     ret = res.retval.retval;
     return ret;
    
}
} // end of namespace
namespace backend {
inline void Hello__reformatGreetings::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Hello>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
       res.retval.retval = 
            self->reformatGreetings(req.args.gs);
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("Hello__reformatGreetings", std::make_shared<Hello__reformatGreetings>());
inline void Hello__register_hello_cb::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      req.args.cb.activate(comm, ws);

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Hello>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
       res.retval.retval = 
            self->register_hello_cb(req.args.cb);
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("Hello__register_hello_cb", std::make_shared<Hello__register_hello_cb>());
inline void Hello__sayHello::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Hello>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
       res.retval.retval = 
            self->sayHello(req.args.weSay);
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("Hello__sayHello", std::make_shared<Hello__sayHello>());
}
namespace backend {
inline void HelloCB__confirmHello::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<HelloCB>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
       res.retval.retval = 
            self->confirmHello(req.args.hello);
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("HelloCB__confirmHello", std::make_shared<HelloCB__confirmHello>());
}
#endif

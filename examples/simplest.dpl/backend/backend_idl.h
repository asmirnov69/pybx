// -*- c++ -*-
// generated code: source - ./examples/simplest.dpl/backend/backend_idl.py
#ifndef __BACKEND_IDL_STUBS_HH__
#define __BACKEND_IDL_STUBS_HH__

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE static int TOKENPASTE2(Unique_, __LINE__)

#include <memory>
#include <string>
#include <stdexcept>
using namespace std;

#include <kvan/json-io.h>
#include <libdipole/communicator.h>
#include <libdipole/proto.h>
#include <libdipole/remote-methods.h>
    
enum class Color {
NORMAL = 0,
RED,
GREEN,
};
template <> inline std::string get_enum_value_string<Color>(Color v) {
 std::string ret;
 switch (v) {
  case Color::NORMAL: ret = "NORMAL"; break;
  case Color::RED: ret = "RED"; break;
  case Color::GREEN: ret = "GREEN"; break;
  }
 return ret;
}
template <> inline void set_enum_value<Color>(Color* v, const std::string& new_v)
{
 if (new_v == "NORMAL") *v = Color::NORMAL;
 else if (new_v == "RED") *v = Color::RED;
 else if (new_v == "GREEN") *v = Color::GREEN;
 else {
  std::ostringstream m;
  m << "set_enum_value for Color: unknown string " << new_v;
  throw runtime_error(m.str());
 }
}
struct Greetings {
 std::string language;
 std::string text;
 Color color;
};
template <> inline StructDescriptor get_struct_descriptor<Greetings>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("language", &Greetings::language),
  make_member_descriptor("text", &Greetings::text),
  make_member_descriptor("color", &Greetings::color),
 };
 return sd;
}
class HelloPtr;
class HelloCBPtr;
typedef vector<Greetings> GreetingsSeq;
class HelloPtr {
private:
  Dipole::Communicator* comm{nullptr};
  std::shared_ptr<ix::WebSocket> ws;
public:
  std::string object_id;
  std::string ws_url;
  HelloPtr();
  HelloPtr(Dipole::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);
  HelloPtr(Dipole::Communicator* comm, const std::string& object_id);
  void activate(Dipole::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);
  Greetings sayHello(std::string weSay);
  GreetingsSeq reformatGreetings(GreetingsSeq gs);
  std::string register_hello_cb(HelloCBPtr cb);
};
template <> inline StructDescriptor get_struct_descriptor<HelloPtr>()
{
 static const StructDescriptor sd = {
   make_member_descriptor("object_id", &HelloPtr::object_id),
   make_member_descriptor("ws_url", &HelloPtr::ws_url),
 };
 return sd;
}
class HelloCBPtr {
private:
  Dipole::Communicator* comm{nullptr};
  std::shared_ptr<ix::WebSocket> ws;
public:
  std::string object_id;
  std::string ws_url;
  HelloCBPtr();
  HelloCBPtr(Dipole::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);
  HelloCBPtr(Dipole::Communicator* comm, const std::string& object_id);
  void activate(Dipole::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);
  std::string confirmHello(std::string hello);
};
template <> inline StructDescriptor get_struct_descriptor<HelloCBPtr>()
{
 static const StructDescriptor sd = {
   make_member_descriptor("object_id", &HelloCBPtr::object_id),
   make_member_descriptor("ws_url", &HelloCBPtr::ws_url),
 };
 return sd;
}
class Hello : public Dipole::Object {
public:
 typedef HelloPtr ptr;
 virtual Greetings sayHello(std::string weSay) = 0;
 virtual GreetingsSeq reformatGreetings(GreetingsSeq gs) = 0;
 virtual std::string register_hello_cb(HelloCBPtr cb) = 0;
};
struct Hello__sayHello : public Dipole::method_impl
{
 struct args_t {
 std::string weSay;
 };
 struct return_t {
   Greetings retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
template <> inline StructDescriptor get_struct_descriptor<Hello__sayHello::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("weSay", &Hello__sayHello::args_t::weSay),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Request<Hello__sayHello::args_t>>()
{
 return get_StructDescriptor_T<Hello__sayHello::args_t, Dipole::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<Hello__sayHello::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("ret", &Hello__sayHello::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Response<Hello__sayHello::return_t>>()
{
 return get_StructDescriptor_T<Hello__sayHello::return_t, Dipole::Response>::get_struct_descriptor();
}
struct Hello__reformatGreetings : public Dipole::method_impl
{
 struct args_t {
 GreetingsSeq gs;
 };
 struct return_t {
   GreetingsSeq retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
template <> inline StructDescriptor get_struct_descriptor<Hello__reformatGreetings::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("gs", &Hello__reformatGreetings::args_t::gs),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Request<Hello__reformatGreetings::args_t>>()
{
 return get_StructDescriptor_T<Hello__reformatGreetings::args_t, Dipole::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<Hello__reformatGreetings::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("ret", &Hello__reformatGreetings::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Response<Hello__reformatGreetings::return_t>>()
{
 return get_StructDescriptor_T<Hello__reformatGreetings::return_t, Dipole::Response>::get_struct_descriptor();
}
struct Hello__register_hello_cb : public Dipole::method_impl
{
 struct args_t {
 HelloCBPtr cb;
 };
 struct return_t {
   std::string retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
template <> inline StructDescriptor get_struct_descriptor<Hello__register_hello_cb::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("cb", &Hello__register_hello_cb::args_t::cb),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Request<Hello__register_hello_cb::args_t>>()
{
 return get_StructDescriptor_T<Hello__register_hello_cb::args_t, Dipole::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<Hello__register_hello_cb::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("ret", &Hello__register_hello_cb::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Response<Hello__register_hello_cb::return_t>>()
{
 return get_StructDescriptor_T<Hello__register_hello_cb::return_t, Dipole::Response>::get_struct_descriptor();
}
class HelloCB : public Dipole::Object {
public:
 typedef HelloCBPtr ptr;
 virtual std::string confirmHello(std::string hello) = 0;
};
struct HelloCB__confirmHello : public Dipole::method_impl
{
 struct args_t {
 std::string hello;
 };
 struct return_t {
   std::string retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
template <> inline StructDescriptor get_struct_descriptor<HelloCB__confirmHello::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("hello", &HelloCB__confirmHello::args_t::hello),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Request<HelloCB__confirmHello::args_t>>()
{
 return get_StructDescriptor_T<HelloCB__confirmHello::args_t, Dipole::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<HelloCB__confirmHello::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("ret", &HelloCB__confirmHello::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<Dipole::Response<HelloCB__confirmHello::return_t>>()
{
 return get_StructDescriptor_T<HelloCB__confirmHello::return_t, Dipole::Response>::get_struct_descriptor();
}
inline HelloPtr::HelloPtr()
{
}
inline HelloPtr::HelloPtr(Dipole::Communicator* comm,
std::shared_ptr<ix::WebSocket> ws,
const std::string& ws_url, const std::string& object_id)
{
 this->comm = comm;
 this->ws = ws;
 this->ws_url = ws_url;
 this->object_id = object_id;
}
inline HelloPtr::HelloPtr(Dipole::Communicator* comm, const std::string& object_id)
{
 this->comm = comm;
 this->object_id = object_id;
}
inline void HelloPtr::activate(Dipole::Communicator* c, std::shared_ptr<ix::WebSocket> ws)

    {
      this->comm = c;
      if (this->ws == nullptr) {
        if (ws_url == "") {
          this->ws = ws;
        } else {
          throw runtime_error("HelloPtr::activate: not implemented for universal ptr");
        }
      }
    }
    
inline Greetings HelloPtr::sayHello(std::string weSay)
{

    Dipole::Request<Hello__sayHello::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "Hello__sayHello",
      .object_id = object_id,
      .args = Hello__sayHello::args_t()
      };

    req.args.weSay=weSay;
    Greetings ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    Dipole::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    Dipole::Response<Hello__sayHello::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
    
}
inline GreetingsSeq HelloPtr::reformatGreetings(GreetingsSeq gs)
{

    Dipole::Request<Hello__reformatGreetings::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "Hello__reformatGreetings",
      .object_id = object_id,
      .args = Hello__reformatGreetings::args_t()
      };

    req.args.gs=gs;
    GreetingsSeq ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    Dipole::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    Dipole::Response<Hello__reformatGreetings::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
    
}
inline std::string HelloPtr::register_hello_cb(HelloCBPtr cb)
{

    Dipole::Request<Hello__register_hello_cb::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "Hello__register_hello_cb",
      .object_id = object_id,
      .args = Hello__register_hello_cb::args_t()
      };

    req.args.cb=cb;
    std::string ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    Dipole::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    Dipole::Response<Hello__register_hello_cb::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
    
}
inline HelloCBPtr::HelloCBPtr()
{
}
inline HelloCBPtr::HelloCBPtr(Dipole::Communicator* comm,
std::shared_ptr<ix::WebSocket> ws,
const std::string& ws_url, const std::string& object_id)
{
 this->comm = comm;
 this->ws = ws;
 this->ws_url = ws_url;
 this->object_id = object_id;
}
inline HelloCBPtr::HelloCBPtr(Dipole::Communicator* comm, const std::string& object_id)
{
 this->comm = comm;
 this->object_id = object_id;
}
inline void HelloCBPtr::activate(Dipole::Communicator* c, std::shared_ptr<ix::WebSocket> ws)

    {
      this->comm = c;
      if (this->ws == nullptr) {
        if (ws_url == "") {
          this->ws = ws;
        } else {
          throw runtime_error("HelloCBPtr::activate: not implemented for universal ptr");
        }
      }
    }
    
inline std::string HelloCBPtr::confirmHello(std::string hello)
{

    Dipole::Request<HelloCB__confirmHello::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "HelloCB__confirmHello",
      .object_id = object_id,
      .args = HelloCB__confirmHello::args_t()
      };

    req.args.hello=hello;
    std::string ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    Dipole::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    Dipole::Response<HelloCB__confirmHello::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
    
}
inline void Hello__sayHello::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      Dipole::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Hello>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      Dipole::Response<return_t> res;
      res.message_id = Dipole::create_new_message_id();
      res.orig_message_id = req.message_id;
      res.retval.retval = self->sayHello(req.args.weSay);
      to_json(res_os, res);
    } catch (exception& e) {
      Dipole::ExceptionResponse eres;
      eres.message_id = Dipole::create_new_message_id();
      eres.orig_message_id = Dipole::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = Dipole::RemoteMethods::register_method("Hello__sayHello", std::make_shared<Hello__sayHello>());
inline void Hello__reformatGreetings::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      Dipole::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Hello>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      Dipole::Response<return_t> res;
      res.message_id = Dipole::create_new_message_id();
      res.orig_message_id = req.message_id;
      res.retval.retval = self->reformatGreetings(req.args.gs);
      to_json(res_os, res);
    } catch (exception& e) {
      Dipole::ExceptionResponse eres;
      eres.message_id = Dipole::create_new_message_id();
      eres.orig_message_id = Dipole::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = Dipole::RemoteMethods::register_method("Hello__reformatGreetings", std::make_shared<Hello__reformatGreetings>());
inline void Hello__register_hello_cb::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      Dipole::Request<args_t> req;
      from_json(&req, req_s);

      req.args.cb.activate(comm, ws);

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Hello>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      Dipole::Response<return_t> res;
      res.message_id = Dipole::create_new_message_id();
      res.orig_message_id = req.message_id;
      res.retval.retval = self->register_hello_cb(req.args.cb);
      to_json(res_os, res);
    } catch (exception& e) {
      Dipole::ExceptionResponse eres;
      eres.message_id = Dipole::create_new_message_id();
      eres.orig_message_id = Dipole::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = Dipole::RemoteMethods::register_method("Hello__register_hello_cb", std::make_shared<Hello__register_hello_cb>());
inline void HelloCB__confirmHello::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      Dipole::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<HelloCB>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      Dipole::Response<return_t> res;
      res.message_id = Dipole::create_new_message_id();
      res.orig_message_id = req.message_id;
      res.retval.retval = self->confirmHello(req.args.hello);
      to_json(res_os, res);
    } catch (exception& e) {
      Dipole::ExceptionResponse eres;
      eres.message_id = Dipole::create_new_message_id();
      eres.orig_message_id = Dipole::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = Dipole::RemoteMethods::register_method("HelloCB__confirmHello", std::make_shared<HelloCB__confirmHello>());
#endif

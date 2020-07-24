// -*- c++ -*-
// generated code: source - backend_idl.py
#ifndef __BACKEND_IDL_STUB_HH__
#define __BACKEND_IDL_STUB_HH__

#include <memory>
#include <string>
#include <stdexcept>
using namespace std;

#include <kvan/json-io.h>
#include <libdipole/communicator.h>
#include <libdipole/proto.h>
#include <libdipole/remote-methods.h>

// stubs

enum class Color {
  NORMAL = 0,
  RED,
  GREEN
};

template <> inline string get_enum_value_string<Color>(Color v) {
  string ret;
  switch (v) {
  case Color::NORMAL: ret = "NORMAL"; break;
  case Color::RED: ret = "RED"; break;
  case Color::GREEN: ret = "GREEN"; break;
  }
  return ret;
}

template <> inline void set_enum_value<Color>(Color* v, const string& new_v)
{
  if (new_v == "NORMAL") *v = Color::NORMAL;
  else if (new_v == "RED") *v = Color::RED;
  else if (new_v == "GREEN") *v = Color::GREEN;
  else {
    ostringstream m;
    m << "set_enum_value for Color: unknown string " << new_v;
    throw runtime_error(m.str());    
  }
}

struct Greetings {
  string language;
  string text;
  Color color;
};

template <> inline StructDescriptor get_struct_descriptor<Greetings>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("language", &Greetings::language),
    make_member_descriptor("text", &Greetings::text),
    make_member_descriptor("color", &Greetings::color)
  };
  return sd;
}

class HelloPtr;
class HelloCBPtr;

class HelloPtr
{
private:
  Dipole::Communicator* comm{nullptr};
  shared_ptr<ix::WebSocket> ws;
  
public:
  string object_id;
  string ws_url;

  HelloPtr();
  HelloPtr(Dipole::Communicator* comm, shared_ptr<ix::WebSocket> ws,
	   const string& ws_url, const string& object_id);
  HelloPtr(Dipole::Communicator* comm, const string& object_id);
  Greetings sayHello(string weSay);
  string register_hello_cb(HelloCBPtr cb);
};

template <> inline
StructDescriptor get_struct_descriptor<HelloPtr>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("object_id", &HelloPtr::object_id),
    make_member_descriptor("ws_url", &HelloPtr::ws_url)
  };
  return sd;
}

class HelloCBPtr
{
private:
  Dipole::Communicator* comm{nullptr};
  shared_ptr<ix::WebSocket> ws;
  
public:
  string object_id;
  string ws_url;
  HelloCBPtr();
  HelloCBPtr(Dipole::Communicator* comm, shared_ptr<ix::WebSocket> ws,
	     const string& ws_url, const string& object_id);
  HelloCBPtr(Dipole::Communicator* comm, const string& object_id);
  void activate(Dipole::Communicator*, shared_ptr<ix::WebSocket> ws);
  string confirmHello(string hello);
};

template <> inline
StructDescriptor get_struct_descriptor<HelloCBPtr>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("object_id", &HelloCBPtr::object_id),
    make_member_descriptor("ws_url", &HelloCBPtr::ws_url)
  };
  return sd;
}

class Hello : public Dipole::Object {
public:
  typedef HelloPtr ptr;
  virtual Greetings sayHello(string weSay) = 0;
  virtual string register_hello_cb(HelloCBPtr cb) = 0;
};

// Hello::sayHello(self, weSay: str) -> Greetings
struct Hello__sayHello : public Dipole::method_impl
{
  struct args_t {
    string weSay;
  };
  
  struct return_t {
    Greetings retval;
  };

  void do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket>) override
  {
    Dipole::Request<args_t> req;
    from_json(&req, req_s);
    
    auto o = comm->find_object(req.object_id);
    ostringstream res_os;
    if (auto self = dynamic_pointer_cast<Hello>(o);
	self != nullptr) {
      try {
	Dipole::Response<return_t> res;
	res.message_id = Dipole::create_new_message_id();
	res.orig_message_id = req.message_id;
	res.retval.retval = self->sayHello(req.args.weSay);
	to_json(res_os, res);
      } catch (exception& e) {
	Dipole::ExceptionResponse eres;
	eres.message_id = Dipole::create_new_message_id();
	eres.orig_message_id = req.message_id;
	eres.remote_exception_text = e.what();
	to_json(res_os, eres);
      }
    } else {
      throw runtime_error("dyn type mismatch");
    }

    *res_s = res_os.str();
  }
};

template <> inline StructDescriptor
get_struct_descriptor<Hello__sayHello::args_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("weSay", &Hello__sayHello::args_t::weSay)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Request<Hello__sayHello::args_t>>()
{
  return get_StructDescriptor_T<Hello__sayHello::args_t, Dipole::Request>::get_struct_descriptor();
}

template <> inline StructDescriptor
get_struct_descriptor<Hello__sayHello::return_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("ret", &Hello__sayHello::return_t::retval)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Response<Hello__sayHello::return_t>>()
{
  return get_StructDescriptor_T<Hello__sayHello::return_t, Dipole::Response>::get_struct_descriptor();
}

// Hello::register_hello_cb(self, cb: HelloCBPtr) -> None
struct Hello__register_hello_cb : public Dipole::method_impl
{
  struct args_t {
    HelloCBPtr cb;
  };
  
  struct return_t {
    string retval;
  };

  void do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws) override  
  {
    Dipole::Request<args_t> req;
    from_json(&req, req_s);

    req.args.cb.activate(comm, ws);
    
    auto o = comm->find_object(req.object_id);
    ostringstream res_os;
    if (auto self = dynamic_pointer_cast<Hello>(o);
	self != nullptr) {
      try {
	Dipole::Response<return_t> res;
	res.message_id = Dipole::create_new_message_id();
	res.orig_message_id = req.message_id;
	res.retval.retval = self->register_hello_cb(req.args.cb);
	to_json(res_os, res);
      } catch (exception& e) {
	Dipole::ExceptionResponse eres;
	eres.message_id = Dipole::create_new_message_id();
	eres.orig_message_id = req.message_id;
	eres.remote_exception_text = e.what();
	to_json(res_os, eres);
      }
    } else {
      throw runtime_error("dyn type mismatch");
    }

    *res_s = res_os.str();
  }
};

template <> inline StructDescriptor
get_struct_descriptor<Hello__register_hello_cb::args_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("cb", &Hello__register_hello_cb::args_t::cb)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Request<Hello__register_hello_cb::args_t>>()
{
  return get_StructDescriptor_T<Hello__register_hello_cb::args_t, Dipole::Request>::get_struct_descriptor();
}

template <> inline StructDescriptor
get_struct_descriptor<Hello__register_hello_cb::return_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("retval", &Hello__register_hello_cb::return_t::retval)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Response<Hello__register_hello_cb::return_t>>()
{
  return get_StructDescriptor_T<Hello__register_hello_cb::return_t, Dipole::Response>::get_struct_descriptor();
}

// ptr
inline HelloPtr::HelloPtr()
{
}

inline HelloPtr::HelloPtr(Dipole::Communicator* comm,
			  shared_ptr<ix::WebSocket> ws,
			  const string& ws_url, const string& object_id)
{
  this->comm = comm;
  this->ws = ws;
  this->ws_url = ws_url;
  this->object_id = object_id;
}

inline HelloPtr::HelloPtr(Dipole::Communicator* comm, const string& object_id)
{
  this->comm = comm;
  this->object_id = object_id;
}

inline Greetings HelloPtr::sayHello(string weSay)
{
  Dipole::Request<Hello__sayHello::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "Hello__sayHello",
      .object_id = object_id,
      .args = Hello__sayHello::args_t()
      };
  req.args.weSay = weSay;
  Greetings ret;
  
  ostringstream json_os;
  to_json(json_os, req);
  ws->sendBinary(json_os.str());
  cout << "HelloPtr::sayHello: msg send: " << json_os.str() << endl;
  auto res_s = comm->wait_for_response(req.message_id);
  comm->check_response(res_s.first, res_s.second);
  
  Dipole::Response<Hello__sayHello::return_t> res;
  from_json(&res, res_s.second);
  ret = res.retval.retval;
  return ret;
}

inline string HelloPtr::register_hello_cb(HelloCBPtr cb)
{
  Dipole::Request<Hello__register_hello_cb::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "Hello__register_hello_cb",
      .object_id = object_id,
      .args = Hello__register_hello_cb::args_t()
      };
  req.args.cb = cb;
  string ret;
  
  ostringstream json_os;
  to_json(json_os, req);
  ws->sendBinary(json_os.str());
  cout << "HelloPtr::register_hello_cb: msg sent: " << json_os.str() << endl;
  auto res_s = comm->wait_for_response(req.message_id);
  comm->check_response(res_s.first, res_s.second);
  
  Dipole::Response<Hello__register_hello_cb::return_t> res;
  from_json(&res, res_s.second);
  ret = res.retval.retval;
  return ret;
}

// class HelloCB
class HelloCBPtr;
class HelloCB : public Dipole::Object {
public:
  typedef HelloCBPtr ptr;
  virtual string confirmHello(string hello) = 0;
};

// HelloCB::confirmHello(self, hello: str) -> str
struct HelloCB__confirmHello : public Dipole::method_impl
{
  struct args_t {
    string hello;
  };
  
  struct return_t {
    string retval;
  };

  void do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket>) override
  {
    Dipole::Request<args_t> req;
    from_json(&req, req_s);
    
    auto o = comm->find_object(req.object_id);
    ostringstream res_os;
    if (auto self = dynamic_pointer_cast<HelloCB>(o);
	self != nullptr) {
      try {
	Dipole::Response<return_t> res;
	res.message_id = Dipole::create_new_message_id();
	res.orig_message_id = req.message_id;
	res.retval.retval = self->confirmHello(req.args.hello);
	to_json(res_os, res);
      } catch (exception& e) {
	Dipole::ExceptionResponse eres;
	eres.message_id = Dipole::create_new_message_id();
	eres.orig_message_id = req.message_id;
	eres.remote_exception_text = e.what();
	to_json(res_os, eres);
      }
    } else {
      throw runtime_error("dyn type mismatch");
    }

    *res_s = res_os.str();
  }
};

template <> inline StructDescriptor
get_struct_descriptor<HelloCB__confirmHello::args_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("hello", &HelloCB__confirmHello::args_t::hello)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Request<HelloCB__confirmHello::args_t>>()
{
  return get_StructDescriptor_T<HelloCB__confirmHello::args_t, Dipole::Request>::get_struct_descriptor();
}

template <> inline StructDescriptor
get_struct_descriptor<HelloCB__confirmHello::return_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("ret", &HelloCB__confirmHello::return_t::retval)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Response<HelloCB__confirmHello::return_t>>()
{
  return get_StructDescriptor_T<HelloCB__confirmHello::return_t, Dipole::Response>::get_struct_descriptor();
}

// ptr
inline HelloCBPtr::HelloCBPtr()
{
  this->comm = nullptr;
}

inline HelloCBPtr::HelloCBPtr(Dipole::Communicator* comm,
			      shared_ptr<ix::WebSocket> ws,
			      const string& ws_url, const string& object_id)
{
  this->comm = comm;
  this->ws = ws;
  this->ws_url = ws_url;
  this->object_id = object_id;
}

inline HelloCBPtr::HelloCBPtr(Dipole::Communicator* comm,
			      const string& object_id)
{
  this->comm = comm;
  this->object_id = object_id;
}

inline void HelloCBPtr::activate(Dipole::Communicator* c,
				 shared_ptr<ix::WebSocket> ws)
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

inline string HelloCBPtr::confirmHello(string hello)
{
  cout << "HelloCBPtr::confirmHello: waiters: " << comm->waiters.size() << endl;
  Dipole::Request<HelloCB__confirmHello::args_t> req{
    .message_type = Dipole::message_type_t::METHOD_CALL,
      .message_id = Dipole::create_new_message_id(),
      .method_signature = "HelloCB__confirmHello",
      .object_id = object_id,
      .args = HelloCB__confirmHello::args_t()
      };
  req.args.hello = hello;
  string ret;
  
  ostringstream json_os;
  to_json(json_os, req);  
  cout << "HelloCBPtr::confirmHello: " << json_os.str() << endl;
  cout << "HelloCBPtr::confirmHello: waiters0: " << comm->waiters.size() << endl;
  ws->sendBinary(json_os.str());
  cout << "HelloCBPtr::confirmHello: msg sent: " << json_os.str() << endl;
  cout << "HelloCBPtr::confirmHello: waiters1: " << comm->waiters.size() << endl;
  auto res_s = comm->wait_for_response(req.message_id);
  cout << "HelloCBPtr::confirmHello: waiters2: " << comm->waiters.size() << endl;
  comm->check_response(res_s.first, res_s.second);
  cout << "HelloCBPtr::confirmHello: waiters3: " << comm->waiters.size() << endl;
  
  Dipole::Response<HelloCB__confirmHello::return_t> res;
  from_json(&res, res_s.second);
  ret = res.retval.retval;
  return ret;
}


// register all methods

static int _0 = Dipole::RemoteMethods::register_method("Hello__sayHello", make_shared<Hello__sayHello>());
static int _1 = Dipole::RemoteMethods::register_method("Hello__register_hello_cb", make_shared<Hello__register_hello_cb>());
static int _2 = Dipole::RemoteMethods::register_method("HelloCB__confirmHello", make_shared<HelloCB__confirmHello>());

#endif

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

class HelloPtrImpl;
typedef shared_ptr<HelloPtrImpl> HelloPtr;
class Hello : public Dipole::Object {
public:
  typedef HelloPtr ptr;
  typedef HelloPtrImpl ptr_impl;
  virtual Greetings sayHello(string weSay) = 0;
  //virtual void register_hello_cb(HelloCBPtr cb) = 0;
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

  void do_call(const string& req_s, string* res_s) override
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

// ptr
class HelloPtrImpl
{
private:
  Dipole::Communicator* comm;
  shared_ptr<ix::WebSocket> ws;
  string object_id;
  string ws_url;
  
public:
  HelloPtrImpl(Dipole::Communicator* comm, shared_ptr<ix::WebSocket> ws,
	       const string& ws_url, const string& object_id)
  {
    this->comm = comm;
    this->ws = ws;
    this->ws_url = ws_url;
    this->object_id = object_id;
  }
  HelloPtrImpl(Dipole::Communicator* comm, const string& object_id)
  {
    this->comm = comm;
    this->object_id = object_id;
  }
  
  Greetings sayHello(string weSay) {
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
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    Dipole::Response<Hello__sayHello::return_t> res;
    from_json(&res, res_s.second);
    ret = res.retval.retval;
    return ret;
  }
};

// register all methods

static int _0 = Dipole::RemoteMethods::register_method("Hello__sayHello", make_shared<Hello__sayHello>());

#endif

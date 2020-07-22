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
class Hello : public Dipole::Object {
public:
  typedef HelloPtr ptr;
  virtual Greetings sayHello(string weSay) = 0;
};

// Hello::sayHello(self, weSay: str) -> Greetings
struct Hello__sayHello : public Dipole::method_impl
{
  struct args {
    string weSay;
  };
  
  struct return_t {
    Greetings ret;
  };

  void do_call(const string& req_s, string* res_s) override
  {
    Dipole::Request<args> req;
    from_json(&req, req_s);
    Dipole::Response<return_t> res;
    res.message_id = Dipole::create_new_message_id();
    res.orig_message_id = req.message_id;
    
    auto o = comm->find_object(req.object_id);
    if (auto self = dynamic_pointer_cast<Hello>(o);
	self != nullptr) {
      res.ret.ret = self->sayHello(req.args.weSay);
    } else {
      throw runtime_error("dyn type mismatch");
    }
    ostringstream res_os;
    to_json(res_os, res);
    *res_s = res_os.str();
  }
};

template <> inline StructDescriptor
get_struct_descriptor<Hello__sayHello::args>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("weSay", &Hello__sayHello::args::weSay)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Request<Hello__sayHello::args>>()
{
  return get_StructDescriptor_T<Hello__sayHello::args, Dipole::Request>::get_struct_descriptor();
}

template <> inline StructDescriptor
get_struct_descriptor<Hello__sayHello::return_t>()
{
  static const StructDescriptor sd = {
    make_member_descriptor("ret", &Hello__sayHello::return_t::ret)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Response<Hello__sayHello::return_t>>()
{
  return get_StructDescriptor_T<Hello__sayHello::return_t, Dipole::Response>::get_struct_descriptor();
}

// ptr
class HelloPtr : public Dipole::ObjectPtr
{
private:
  Dipole::Communicator* comm;
  
public:
  HelloPtr(Dipole::Communicator* comm) { this->comm = comm; }
  Greetings sayHello(string weSay) {
    Dipole::Request<Hello__sayHello::args> req{
      .message_type = Dipole::message_type_t::METHOD_CALL,
	.message_id = Dipole::create_new_message_id(),
	.method_signature = "Hello__sayHello",
	.object_id = object_id,
	.args = Hello__sayHello::args()
	};
    req.args.weSay = weSay;
    
    ostringstream json_os;
    to_json(json_os, req);
    ws->sendBinary(json_os.str());
    string res_s = comm->wait_for_response(req.message_id);
    Dipole::Response<Hello__sayHello::return_t> res;
    from_json(&res, res_s);
    if (res.is_remote_exception) {
      throw Dipole::RemoteException("some exception text goes here");
    }
    return res.ret.ret;
  }
};

template<> inline
shared_ptr<HelloPtr>
Dipole::ptr_cast(Communicator* comm, ObjectPtr o_ptr)
{
  auto ret = make_shared<HelloPtr>(comm);
  ret->ws_url = o_ptr.ws_url;
  ret->object_id = o_ptr.object_id;
  ret->ws = o_ptr.ws;
  return ret;
}

// register all methods

static int _0 = Dipole::RemoteMethods::register_method("Hello__sayHello", make_shared<Hello__sayHello>());

#endif

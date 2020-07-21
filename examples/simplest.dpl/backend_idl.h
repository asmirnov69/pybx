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
class HelloPtr;
class HelloCBPtr;
class Hello : public Dipole::Object {
public:
  typedef HelloPtr ptr;
  virtual string sayHello() = 0;
#if 0
  virtual string sayAloha(const string& language) = 0;
  virtual string get_holidays() = 0;
  virtual void register_hello_cb(const HelloCBPtr&) = 0;
#endif
};

class HelloCB : public Dipole::Object {
public:
  virtual string confirmHello(const string& hello) = 0;
};

#if 0
class HelloCBPtr : public Dipole::ObjectPtr
{
public:
  string confirmHello(const string& hello) { req = ...; ws.send(req); add waiting q; }
};
#endif

// Hello::sayHello(self) -> str
struct Hello__sayHello : public Dipole::method_impl
{
  struct args {
  };

  struct return_t {
    string ret;
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
      res.ret.ret = self->sayHello();
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
  //typedef Hello__sayHello::args st;
  StructDescriptor sd = {
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Request<Hello__sayHello::args>>()
{
  typedef Dipole::Request<Hello__sayHello::args> st;
  StructDescriptor sd = {
    make_member_descriptor("message-type", &st::message_type),
    make_member_descriptor("message-id", &st::message_id),
    make_member_descriptor("method-signature", &st::method_signature),
    make_member_descriptor("object-id", &st::object_id),
    make_member_descriptor("args", &st::args)
  };  
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Hello__sayHello::return_t>()
{
  typedef Hello__sayHello::return_t st;
  StructDescriptor sd = {
    make_member_descriptor("ret", &st::ret)
  };
  return sd;
}

template <> inline StructDescriptor
get_struct_descriptor<Dipole::Response<Hello__sayHello::return_t>>()
{
  typedef Dipole::Response<Hello__sayHello::return_t> st;
  StructDescriptor sd = {
    make_member_descriptor("message-type", &st::message_type),
    make_member_descriptor("message-id", &st::message_id),
    make_member_descriptor("orig-message-id", &st::orig_message_id),
    make_member_descriptor("is-remote-exception", &st::is_remote_exception),
    make_member_descriptor("ret", &st::ret)
  };
  return sd;
}

// ptr
class HelloPtr : public Dipole::ObjectPtr
{
private:
  Dipole::Communicator* comm;
  
public:
  HelloPtr(Dipole::Communicator* comm) { this->comm = comm; }
  string sayHello() {
    Dipole::Request<Hello__sayHello::args> req{
      .message_type = Dipole::message_type_t::METHOD_CALL,
	.message_id = Dipole::create_new_message_id(),
	.method_signature = "Hello__sayHello",
	.object_id = object_id,
	.args = Hello__sayHello::args()
	};
    ostringstream json_os;
    to_json(json_os, req);
    ws->sendBinary(json_os.str());
    string res_s = comm->wait_for_response(req.message_id);
    Dipole::Response<Hello__sayHello::return_t> res;
    from_json(&res, res_s);
    if (res.is_remote_exception) {
      throw Dipole::RemoteException(res.ret.ret);
    }
    return res.ret.ret;
  }

#if 0
  string sayAloha(const string& language) { req = ...; ws.send(req); add waiting q; }
  string get_holidays() { req = ...; ws.send(req); add waiting q; }
  void register_hello_cb(const HelloCBPtr&) { req = ...; ws.send(req); add waiting q; }
#endif
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
#if 0
int _1 = Dipole::Methods::register_method("Hello__sayAloha", make_shared<Hello__sayAloha>());
int _2 = Dipole::Methods::register_method("Hello__get_holidays", make_shared<Hello__get_holidays>());
int _3 = Dipole::Methods::register_method("Hello__register_hello_cb", make_shared<Hello__register_hello_cb>());
int _4 = Dipole::Methods::register_method("HelloCB__confirmHello", make_shared<HelloCB__confirmHello>());
#endif

#endif

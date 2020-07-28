// -*- c++ -*-
#ifndef __DIPOLE_PROTO_HH__
#define __DIPOLE_PROTO_HH__

#include <string>
#include <stdexcept>
#include <sstream>
using namespace std;

#include <kvan/enum-io.h>
#include <kvan/struct-descriptor.h>

namespace Dipole {
  enum class message_type_t {
    METHOD_CALL = 0, METHOD_CALL_RETURN, METHOD_CALL_EXCEPTION
  };

  string create_new_message_id();
  message_type_t get_message_type(const string& msg);
  string get_method_signature(const string& msg);
  string get_orig_message_id(const string& msg);
  string get_message_id(const string& msg);

  template <class ARGS> struct Request {
    message_type_t message_type{message_type_t::METHOD_CALL};
    string message_id;
    string method_signature;
    string object_id;
    ARGS args;
  };

  template <class RET> struct Response {
    message_type_t message_type{message_type_t::METHOD_CALL_RETURN};
    string message_id;
    string orig_message_id;
    RET retval;
  };

  struct ExceptionResponse {
    message_type_t message_type{message_type_t::METHOD_CALL_EXCEPTION};
    string message_id;
    string orig_message_id;
    string remote_exception_text;
  };

};

template <> inline string
get_enum_value_string<Dipole::message_type_t>(Dipole::message_type_t t)
{
  string ret;
  switch (t) {
  case Dipole::message_type_t::METHOD_CALL:
    ret = "method-call";
    break;
  case Dipole::message_type_t::METHOD_CALL_RETURN:
    ret = "method-call-return";
    break;
  case Dipole::message_type_t::METHOD_CALL_EXCEPTION:
    ret = "method-call-exception";
    break;
  }
  return ret;
}

template <> inline
void set_enum_value<Dipole::message_type_t>(Dipole::message_type_t* o, const string& v)
{
  if (v == "method-call") {
    *o = Dipole::message_type_t::METHOD_CALL;
  } else if (v == "method-call-return") {
    *o = Dipole::message_type_t::METHOD_CALL_RETURN;
  } else if (v == "method-call-exception") {
    *o = Dipole::message_type_t::METHOD_CALL_EXCEPTION;
  } else {
    ostringstream m;
    m << "set_enum_value<message_type_t>: uknown value " << v;
    throw runtime_error(m.str());
  }
}

template <class T, template <typename> class TT>
struct get_StructDescriptor_T {
  static StructDescriptor get_struct_descriptor() {
    static_assert(assert_false<T>::value, "provide spec");
    return StructDescriptor();
  }
};

template <class ARGS>
struct get_StructDescriptor_T<ARGS, Dipole::Request> {
  static StructDescriptor get_struct_descriptor() {
    typedef Dipole::Request<ARGS> st;
    static const StructDescriptor sd = {
      make_member_descriptor("message-type", &st::message_type),
      make_member_descriptor("message-id", &st::message_id),
      make_member_descriptor("method-signature", &st::method_signature),
      make_member_descriptor("object-id", &st::object_id),
      make_member_descriptor("args", &st::args)
    };  
    return sd;
  }
};

template <class RET>
struct get_StructDescriptor_T<RET, Dipole::Response> {
  static StructDescriptor get_struct_descriptor() {
    typedef Dipole::Response<RET> st;
    static const StructDescriptor sd = {
      make_member_descriptor("message-type", &st::message_type),
      make_member_descriptor("message-id", &st::message_id),
      make_member_descriptor("orig-message-id", &st::orig_message_id),
      make_member_descriptor("retval", &st::retval)
    };  
    return sd;
  }
};  

template <> inline
StructDescriptor get_struct_descriptor<Dipole::ExceptionResponse>()
{
  typedef Dipole::ExceptionResponse st;
  static const StructDescriptor sd = {
    make_member_descriptor("message-type", &st::message_type),
    make_member_descriptor("message-id", &st::message_id),
    make_member_descriptor("orig-message-id", &st::orig_message_id),
    make_member_descriptor("remote-exception-text", &st::remote_exception_text)
  };
  return sd;
}

#endif

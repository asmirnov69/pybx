// -*- c++ -*-
#ifndef __DIPOLE_PROTO_HH__
#define __DIPOLE_PROTO_HH__

#include <string>
#include <stdexcept>
#include <sstream>
using namespace std;

#include <kvan/enum-io.h>
#include <kvan/struct-descriptor.h>

namespace pybx {
  enum class message_type_t {
    METHOD_CALL = 0, METHOD_CALL_RETURN, METHOD_CALL_EXCEPTION, METHOD_ONEWAY_CALL
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
get_enum_value_string<pybx::message_type_t>(pybx::message_type_t t)
{
  string ret;
  switch (t) {
  case pybx::message_type_t::METHOD_CALL:
    ret = "method-call";
    break;
  case pybx::message_type_t::METHOD_CALL_RETURN:
    ret = "method-call-return";
    break;
  case pybx::message_type_t::METHOD_CALL_EXCEPTION:
    ret = "method-call-exception";
    break;
  case pybx::message_type_t::METHOD_ONEWAY_CALL:
    ret = "method-oneway-call";
    break;
  }  
  return ret;
}

template <> inline
void set_enum_value<pybx::message_type_t>(pybx::message_type_t* o, const string& v)
{
  if (v == "method-call") {
    *o = pybx::message_type_t::METHOD_CALL;
  } else if (v == "method-call-return") {
    *o = pybx::message_type_t::METHOD_CALL_RETURN;
  } else if (v == "method-call-exception") {
    *o = pybx::message_type_t::METHOD_CALL_EXCEPTION;
  } else if (v == "method-oneway-call") {
    *o = pybx::message_type_t::METHOD_ONEWAY_CALL;
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
struct get_StructDescriptor_T<ARGS, pybx::Request> {
  static StructDescriptor get_struct_descriptor() {
    typedef pybx::Request<ARGS> st;
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
struct get_StructDescriptor_T<RET, pybx::Response> {
  static StructDescriptor get_struct_descriptor() {
    typedef pybx::Response<RET> st;
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
StructDescriptor get_struct_descriptor<pybx::ExceptionResponse>()
{
  typedef pybx::ExceptionResponse st;
  static const StructDescriptor sd = {
    make_member_descriptor("message-type", &st::message_type),
    make_member_descriptor("message-id", &st::message_id),
    make_member_descriptor("orig-message-id", &st::orig_message_id),
    make_member_descriptor("remote-exception-text", &st::remote_exception_text)
  };
  return sd;
}

#endif

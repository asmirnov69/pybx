// -*- c++ -*-
#ifndef __DIPOLE_PROTO_HH__
#define __DIPOLE_PROTO_HH__

#include <string>
#include <stdexcept>
#include <sstream>
using namespace std;

#include <kvan/enum-io.h>

namespace Dipole {
  enum class message_type_t { METHOD_CALL = 0, METHOD_CALL_RETURN };

  string create_new_message_id();
  message_type_t get_message_type(const string& msg);
  string get_method_signature(const string& msg);
  string get_orig_message_id(const string& msg);

  template <class ARGS> struct Request {
    message_type_t message_type;
    string message_id;
    string method_signature;
    string object_id;
    ARGS args;
  };

  template <class RET> struct Response {
    message_type_t message_type{message_type_t::METHOD_CALL_RETURN};
    string message_id;
    string orig_message_id;
    bool is_remote_exception{false};
    RET ret;
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
  } else {
    ostringstream m;
    m << "set_enum_value<message_type_t>: uknown value " << v;
    throw runtime_error(m.str());
  }
}


#endif

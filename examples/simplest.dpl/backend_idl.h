// -*- c++ -*-
// generated code: source - backend_idl.py
#ifndef __BACKEND_IDL_STUB_HH__
#define __BACKEND_IDL_STUB_HH__

#include <memory>
#include <string>
#include <stdexcept>
using namespace std;

#include <kvan/vector-json-io.h>
#include "dipole.h"

// stubs

class Hello : public ServerObject {
public:
  virtual string sayHello() = 0;
  virtual string sayAloha(const string& language) = 0;
  virtual string get_holidays() = 0;
};

// Hello::sayHello(self) -> str
struct destination_0
{
  struct req {
    int object_id;
    // no args
  };

  struct res {
    string res;
  };

  static void do_call(shared_ptr<ServerObject> o, const req& request, res* response) {
    if (auto self = dynamic_pointer_cast<Hello>(o);
	self != nullptr) {
      response->res = self->sayHello();
    } else {
      throw runtime_error("dyn type mismatch");
    }
  }
};

template <> inline
StructDescriptor get_struct_descriptor<destination_0::req>()
{
  StructDescriptor sd;
  sd.add_member("object_id", &destination_0::req::object_id);
  return sd;
}

template <> inline
StructDescriptor get_struct_descriptor<destination_0::res>()
{
  StructDescriptor sd;
  sd.add_member("object_id", &destination_0::res::res);
  return sd;
}

// Hello::sayAloha(self, language: str) -> str
struct destination_1
{
  struct req {
    int object_id;
    string language;
  };

  struct res {
    string res;
  };

  static void do_call(shared_ptr<ServerObject> o, const req& request, res* response) {
    if (auto self = dynamic_pointer_cast<Hello>(o);
	self != nullptr) {
      response->res = self->sayAloha(request.language);
    } else {
      throw runtime_error("dyn type mismatch");
    }
  }
};

template <> inline
StructDescriptor get_struct_descriptor<destination_1::req>()
{
  StructDescriptor sd;
  sd.add_member("object_id", &destination_1::req::object_id);
  sd.add_member("language", &destination_1::req::language);
  return sd;
}

template <> inline
StructDescriptor get_struct_descriptor<destination_1::res>()
{
  StructDescriptor sd;
  sd.add_member("object_id", &destination_1::res::res);
  return sd;
}


// Hello::get_holidays(self) -> str
struct destination_2
{
  struct req {
    int object_id;
  };

  struct res {
    string res;
  };

  static void do_call(shared_ptr<ServerObject> o, const req& request, res* response) {
    if (auto self = dynamic_pointer_cast<Hello>(o);
	self != nullptr) {
      response->res = self->get_holidays();
    } else {
      throw runtime_error("dyn type mismatch");
    }
  }
};

template <> inline
StructDescriptor get_struct_descriptor<destination_2::req>()
{
  StructDescriptor sd;
  sd.add_member("object_id", &destination_2::req::object_id);
  return sd;
}

template <> inline
StructDescriptor get_struct_descriptor<destination_2::res>()
{
  StructDescriptor sd;
  sd.add_member("object_id", &destination_2::res::res);
  return sd;
}

// dipole server dispatch

inline void dispatch(shared_ptr<ServerObjectManager> om, const string& incoming_msg, string* res_s)
{
  auto jv = from_json(incoming_msg);
  map<string, string> j(jv.begin(), jv.end());
  auto object_id = j["object-id"];
  shared_ptr<ServerObject> object = om->lookup(object_id);
  
  int destination_id = stoi(j["destination-id"]);
  switch (destination_id) {
  case 0:
    {
      destination_0::req req; // no args
      destination_0::res res;
      destination_0::do_call(object, req, &res);
      ostringstream res_str; to_json<destination_0::res>(res_str, res);
      *res_s = res_str.str();
    }
    break;
  case 1:
    {
      destination_1::req req;
#if 0
      for (auto kv: j.find_kvs("args")) {
	if (kv.key == "language") {
	  req.language = kv.value;
	}
      }
#endif
    destination_1::res res;
    destination_1::do_call(object, req, &res);
    ostringstream res_str; to_json<destination_1::res>(res_str, res);
    *res_s = res_str.str();
    }
    break;
  case 2:
    {
      destination_2::req req; // no args
      destination_2::res res;
      destination_2::do_call(object, req, &res);
      ostringstream res_str; to_json<destination_2::res>(res_str, res);
      *res_s = res_str.str();
    }
    break;
  }
}

#endif

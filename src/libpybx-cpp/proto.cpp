#include <iostream>
#include <atomic>
#include <regex>
#include <sstream>
using namespace std;

#include <kvan/uuid.h>
#include <libpybx-cpp/proto.h>

static atomic<int> max_message_id{0};

string pybx::create_new_message_id()
{
  return uuid::generate_uuid_v4();
}

pybx::message_type_t pybx::get_message_type(const string& msg)
{  
  static const regex message_type_re(R"D("message-type": ?"([\w-]+)")D");
  smatch ma;
  if (regex_search(msg, ma, message_type_re)) {
    if (ma.ready() && ma.size() == 2) {
      cout << "pybx::get_message_type: " << ma[1].str() << endl;
      pybx::message_type_t ret;
      set_enum_value(&ret, ma[1].str());
      return ret;
    }
  }

  ostringstream m;
  m << "pybx::get_message_type: can' find message-type: " << msg;
  throw runtime_error(m.str());
}

string pybx::get_method_signature(const string& msg)
{
  static const regex method_signature_re(R"D("method-signature": ?"([\w:]+)")D");
  smatch ma;
  if (regex_search(msg, ma, method_signature_re)) {
    if (ma.ready() && ma.size() == 2) {
      cout << "pybx::get_method_signature: " << ma[1].str() << endl;
      return ma[1];
    }
  }

  ostringstream m;
  m << "pybx::get_method_signature: can' find method-signature: " << msg;
  throw runtime_error(m.str());
}

string pybx::get_orig_message_id(const string& msg)
{
  static const regex orig_message_id_re(R"D("orig-message-id": ?"([\w-]+)")D");
  smatch ma;
  if (regex_search(msg, ma, orig_message_id_re)) {
    if (ma.ready() && ma.size() == 2) {
      return ma[1];
    }
  }

  ostringstream m;
  m << "pybx::get_orig_message_id: can' find orig_message_id: " << msg;
  throw runtime_error(m.str());
}

string pybx::get_message_id(const string& msg)
{
  static const regex message_id_re(R"D("message-id": ?"([\w-]+)")D");
  smatch ma;
  if (regex_search(msg, ma, message_id_re)) {
    if (ma.ready() && ma.size() == 2) {
      return ma[1];
    }
  }

  ostringstream m;
  m << "pybx::get_message_id: can' find message_id: " << msg;
  throw runtime_error(m.str());
}

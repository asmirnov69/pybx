#include "HelloI.h"

#if 0
string HelloCBI::confirmHello(const string& hello)
{
  return string("confirmed: ") + hello;
}
#endif

#if 0
void HelloI::register_hello_cb(const HelloCBPtr& cb_ptr)
{
  cbs.push_back(cb_ptr);
}
#endif

string HelloI::sayHello() {
  //for (auto& cb: cbs) {
  //  cb.confirmHello("Hello");
  //}
  return "Hello";
}

#if 0
string HelloI::sayAloha(const string& language) {
  string ret;
  if (language == "hawaii") {
    ret = "Aloha";
  } else {
    ret = "Privet";
  }
  return ret;
}

string HelloI::get_holidays() {
  return "2020-01-01";
}
#endif

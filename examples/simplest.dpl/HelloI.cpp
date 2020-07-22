#include "HelloI.h"

#if 0
string HelloCBI::confirmHello(const string& hello)
{
  return string("confirmed: ") + hello;
}

void HelloI::register_hello_cb(const HelloCBPtr& cb_ptr)
{
  cbs.push_back(cb_ptr);
}
#endif

Greetings HelloI::sayHello(string weSay) {
  //for (auto& cb: cbs) {
  //  cb.confirmHello("Hello");
  //}
  
  if (weSay != "hi") {
    ostringstream m;
    m << "unexpected weSay value: " << weSay;
    throw runtime_error(m.str());
  }
  cout << "Hello::sayHello" << endl;
  Greetings ret;
  ret.language = "english";
  ret.text = "Hello";
  ret.color = Color::NORMAL;
  return ret;
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

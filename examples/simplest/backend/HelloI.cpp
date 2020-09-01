//#include <unistd.h>
#include "HelloI.h"

string HelloCBI::confirmHello(string hello)
{
  cout << "HelloCBI::confirmHello" << endl;
  return string("confirmed: ") + hello;
}

string HelloI::register_hello_cb(backend::HelloCB_rop cb_rop)
{
  cbs.push_back(cb_rop);
  return "hkjhjhk";
}

backend::Greetings HelloI::sayHello(string weSay) {
  cout << "HelloI::sayHello, size of cbs: " << cbs.size() << endl;
  for (auto& cb: cbs) {
    //cout << cb.ws << endl;
    try {
      cb.confirmHello("Hello");
    } catch (runtime_error& ex) {
      cout << "ignore bad socket" << endl;
    }
  }
  
  if (weSay != "hi") {
    ostringstream m;
    m << "unexpected weSay value: " << weSay;
    throw runtime_error(m.str());
  }
  cout << "Hello::sayHello" << endl;
  backend::Greetings ret;
  ret.language = "english";
  ret.text = "Hello";
  ret.color = backend::Color::NORMAL;
  //sleep(100);
  return ret;
}

vector<backend::Greetings> HelloI::reformatGreetings(vector<backend::Greetings> gs)
{
  cout << "HelloI::reformatGreetings: " << gs.size() << " greetings" << endl;
  for (auto& el: gs) {
    el.text = el.text + " " + el.text;
    el.color = backend::Color::RED;
  }
  return gs;
}

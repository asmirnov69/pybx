#include "HelloI.h"

string HelloCBI::confirmHello(string hello)
{
  cout << "HelloCBI::confirmHello" << endl;
  return string("confirmed: ") + hello;
}

string HelloI::register_hello_cb(HelloCBPtr cb_ptr)
{
  cbs.push_back(cb_ptr);
  return "hkjhjhk";
}

Greetings HelloI::sayHello(string weSay) {
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
  Greetings ret;
  ret.language = "english";
  ret.text = "Hello";
  ret.color = Color::NORMAL;
  return ret;
}

GreetingsSeq HelloI::reformatGreetings(GreetingsSeq gs)
{
  cout << "HelloI::reformatGreetings: " << gs.size() << " greetings" << endl;
  for (auto& el: gs) {
    el.text = el.text + " " + el.text;
    el.color = Color::RED;
  }
  return gs;
}

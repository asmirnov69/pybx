//#include <unistd.h>
#include "HelloI.h"

string HelloCBI::confirmHello(string hello)
{
  cout << "HelloCBI::confirmHello" << endl;
  return string("confirmed: ") + hello;
}

string HelloI::register_hello_cb(pybx::backend::HelloCB_rop cb_rop)
{
  cbs.push_back(cb_rop);
  return "hkjhjhk";
}

pybx::backend::Greetings HelloI::sayHello(string weSay) {
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
  pybx::backend::Greetings ret;
  ret.language = "english";
  ret.text = "Hello";
  ret.color = pybx::backend::Color::NORMAL;
  //sleep(100);
  return ret;
}

vector<pybx::backend::Greetings> HelloI::reformatGreetings(vector<pybx::backend::Greetings> gs)
{
  cout << "HelloI::reformatGreetings: " << gs.size() << " greetings" << endl;
  for (auto& el: gs) {
    el.text = el.text + " " + el.text;
    el.color = pybx::backend::Color::RED;
  }
  return gs;
}

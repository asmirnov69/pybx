// -*- c++ -*-
#ifndef __HELLO_I_HH__
#define __HELLO_I_HH__

#include <vector>
using namespace std;

#include "backend_idl.h"

class HelloI : public Hello
{
private:
  //vector<HelloCBPtr> cbs;
  
public:
  Greetings sayHello(string weSay) override;
  //string sayAloha(const string& language) override;
  //string get_holidays() override;
  //void register_hello_cb(const HelloCBPtr&) override;
};

#if 0
class HelloCBI : public HelloCB
{
public:
  string confirmHello(const string& hello) override;
};
#endif

#endif

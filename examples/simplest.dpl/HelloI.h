// -*- c++ -*-
#ifndef __HELLO_I_HH__
#define __HELLO_I_HH__

#include <vector>
using namespace std;

#include "backend_idl.h"

class HelloI : public Hello
{
private:
  vector<HelloCBPtr> cbs;
  
public:
  Greetings sayHello(string weSay) override;
  string register_hello_cb(HelloCBPtr) override;
};

class HelloCBI : public HelloCB
{
public:
  string confirmHello(string hello) override;
};

#endif

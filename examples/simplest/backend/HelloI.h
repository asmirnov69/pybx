// -*- c++ -*-
#ifndef __HELLO_I_HH__
#define __HELLO_I_HH__

#include <vector>
using namespace std;

#include "backend_pybx.h"

class HelloI : public backend::Hello
{
private:
  vector<backend::HelloCB_rop> cbs;
  
public:
  backend::Greetings sayHello(string weSay) override;
  vector<backend::Greetings> reformatGreetings(vector<backend::Greetings> gs) override;
  string register_hello_cb(backend::HelloCB_rop) override;
};

class HelloCBI : public backend::HelloCB
{
public:
  string confirmHello(string hello) override;
};

#endif

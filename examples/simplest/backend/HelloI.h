// -*- c++ -*-
#ifndef __HELLO_I_HH__
#define __HELLO_I_HH__

#include <vector>
using namespace std;

#include "backend_pybx.h"

class HelloI : public pybx::backend::Hello
{
private:
  vector<pybx::backend::HelloCB_rop> cbs;
  
public:
  pybx::backend::Greetings sayHello(string weSay) override;
  vector<pybx::backend::Greetings> reformatGreetings(vector<pybx::backend::Greetings> gs) override;
  string register_hello_cb(pybx::backend::HelloCB_rop) override;
};

class HelloCBI : public pybx::backend::HelloCB
{
public:
  string confirmHello(string hello) override;
};

#endif

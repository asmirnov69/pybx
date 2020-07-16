// -*- c++ -*-
#ifndef __HELLO_I_HH__
#define __HELLO_I_HH__

#include "backend_idl.h"

class HelloI : public Hello
{
 public:
  string sayHello() override;
  string sayAloha(const string& language) override;
  string get_holidays() override;
};

#endif

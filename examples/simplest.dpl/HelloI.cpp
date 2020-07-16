#include "HelloI.h"

string HelloI::sayHello() {
  return "Hello";
}

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

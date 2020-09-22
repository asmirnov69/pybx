// -*- c++ -*-
#ifndef __PYBX_EXCEPTION_HH__
#define __PYBX_EXCEPTION_HH__

#include <stdexcept>
using namespace std;

namespace pybx {
  class RemoteException : public runtime_error
  {
  public:
    explicit RemoteException(const string& m) :
    runtime_error(string("pybx::RemoteException:\n") + m)
      {}
  };

  class BadROP : public runtime_error
  {
  public:
    explicit BadROP(const string& m) :
    runtime_error(string("pybx::BadROP:\n") + m)
      {}
  };
}

#endif

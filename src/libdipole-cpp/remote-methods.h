// -*- c++ -*-
#ifndef __DIPOLE__REMOTE_METHODS_HH__
#define __DIPOLE__REMOTE_METHODS_HH__

#include <string>
#include <map>
#include <memory>
#include <sstream>
using namespace std;

#include <ixwebsocket/IXWebSocketServer.h>

namespace Dipole {
  class Communicator;
  class method_impl {
    friend class RemoteMethods;
    
  protected:
    Communicator* comm{nullptr};
    
  public:
    virtual void do_call(const string& req_s, string* res_s,
			 shared_ptr<ix::WebSocket>) = 0;
  };
  
  class RemoteMethods
  {
  private:
    static map<string, shared_ptr<method_impl>> remote_methods;
    
  public:
    static int register_method(const string& method_signature, shared_ptr<method_impl>);
    static shared_ptr<method_impl> find_method(const string& method_signature);
    static void set_communicator(Communicator*);
  };
}

#endif

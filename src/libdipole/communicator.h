// -*- c++ -*-
#ifndef __DIPOLE_COMMUNICATOR__HH__
#define __DIPOLE_COMMUNICATOR__HH__

#include <string>
#include <stdexcept>
#include <memory>
using namespace std;

#include <ixwebsocket/IXWebSocketServer.h>

namespace Dipole {

  class Object
  {
  public:
    virtual ~Object() = 0;
  };

  class ObjectPtr
  {
  public:
    string ws_url;
    string object_id;
    shared_ptr<ix::WebSocket> ws;
  };

  class RemoteException : public runtime_error
  {
  public:
    explicit RemoteException(const string& m) : runtime_error(m) {}
  };

  class Communicator;
  class method_impl {
  protected:
    Communicator* comm;
    
  public:
    virtual void do_call(const string& req_s, string* res_s) = 0;
  };
  
  class Methods
  {
  public:
    static int register_method(const string& method_signature, shared_ptr<method_impl>);
    static shared_ptr<method_impl> find_method(const string& method_signature);
    static void set_comminicator(Communicator*);
  };

  class Communicator
  {
  private:
    int listen_port{-1};
    map<string, shared_ptr<Object>> objects; // object_id -> object
    
    class Waiter {};
    map<int, Waiter> waiters; // message_id -> waiter

    void dispatch(shared_ptr<ix::WebSocket> ws, const string& msg);
    void dispatch_method_call(shared_ptr<ix::WebSocket>  ws, const string& msg);
    void dispatch_method_call_return(const string& msg);
    
  public:
    explicit Communicator();
    void set_listen_port(int listen_port);
    string add_object(shared_ptr<Object>, const string& object_id);
    ObjectPtr get_object_ptr(const string& object_id);
    void run();

    ObjectPtr connect(const string& ws_url, const string& obj_id);

    shared_ptr<Object> find_object(const string& object_id);
    string wait_for_response(int message_id);
    void signal_response(int message_id, const string& msg);
  };

  template <class O_ptr> shared_ptr<O_ptr> ptr_cast(Communicator*, ObjectPtr);
};

#endif

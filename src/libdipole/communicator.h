// -*- c++ -*-
#ifndef __DIPOLE_COMMUNICATOR__HH__
#define __DIPOLE_COMMUNICATOR__HH__

#include <string>
#include <stdexcept>
#include <memory>
using namespace std;

#include <ixwebsocket/IXWebSocketServer.h>
#include <kvan/cbq.h>
#include <libdipole/proto.h>

namespace Dipole {

  class Object
  {
  public:
    virtual ~Object() = 0;
  };

  class RemoteException : public runtime_error
  {
  public:
    explicit RemoteException(const string& m) :
      runtime_error(string("remote exception:\n") + m)
    {}
  };

  class Communicator
  {
  private:
    int listen_port{-1};
    map<string, shared_ptr<Object>> objects; // object_id -> object
    
    typedef CBQ<pair<message_type_t, string>, 1> Waiter;
    map<string, Waiter> waiters; // message_id -> waiter

    void dispatch(shared_ptr<ix::WebSocket> ws, const string& msg);
    void dispatch_method_call(shared_ptr<ix::WebSocket>  ws, const string& msg);
    void dispatch_response(message_type_t msg_type, const string& msg);

    shared_ptr<ix::WebSocket> connect(const string& ws_url, const string& object_id);
    string add_object(shared_ptr<Object>, const string& object_id);
    
  public:
    explicit Communicator();
    void set_listen_port(int listen_port);
    void run();

    shared_ptr<Object> find_object(const string& object_id);
    pair<Dipole::message_type_t, string> wait_for_response(const string& message_id);
    void signal_response(const string& message_id, message_type_t msg_type,
			 const string& msg);
    void check_response(message_type_t msg_type, const string& msg);
    
    template <class OBJ_T>
    typename OBJ_T::ptr get_ptr(const string& ws_url, const string& object_id) {
      auto ws = this->connect(ws_url, object_id);
      return make_shared<typename OBJ_T::ptr_impl>(this, ws, ws_url, object_id);
    }

    template <class OBJ_T>
    typename OBJ_T::ptr
    add_object(shared_ptr<Object> o, const string& object_id)
    {
      string real_object_id = this->add_object(o, object_id);
      typename OBJ_T::ptr ret = make_shared<typename OBJ_T::ptr_impl>(this, real_object_id);
      return ret;
    }
  };
};

#endif

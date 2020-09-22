// -*- c++ -*-
#ifndef __DIPOLE_COMMUNICATOR__HH__
#define __DIPOLE_COMMUNICATOR__HH__

#include <string>
#include <stdexcept>
#include <memory>
#include <thread>
using namespace std;

#include <ixwebsocket/IXWebSocketServer.h>
#include <kvan/cbq.h>
#include <libpybx-cpp/exception.h>
#include <libpybx-cpp/proto.h>

namespace pybx {
  
  void ws_send(shared_ptr<ix::WebSocket>, const string& msg);
  
  class Object
  {
  public:
    virtual ~Object() = 0;
  };

  class Communicator
  {
  private:
    int listen_port{-1};

    mutex objects_lock;
    map<string, shared_ptr<Object>> objects; // object_id -> object

    mutex waiters_lock;
    typedef CBQ<pair<message_type_t, string>, 1> Waiter;
    map<string, shared_ptr<Waiter>> waiters; // message_id -> waiter

    void dispatch(shared_ptr<ix::WebSocket> ws, const string& msg);
    void dispatch_method_call(shared_ptr<ix::WebSocket> ws, const string& msg);
    void dispatch_response(message_type_t msg_type, const string& msg);
    void do_dispatch_method_call_thread();
    CBQ<pair<shared_ptr<ix::WebSocket>, string>, 8> worker_q;    
    thread worker_thread;

    shared_ptr<ix::WebSocket> connect(const string& ws_url, const string& object_id);
    string add_object(shared_ptr<Object>, const string& object_id);
    
  public:
    explicit Communicator();
    ~Communicator();
    void set_listen_port(int listen_port);
    void run();

    shared_ptr<Object> find_object(const string& object_id);
    pair<pybx::message_type_t, string>
    send_and_wait_for_response(shared_ptr<ix::WebSocket> ws,
			       const string& req_s,
			       const string& message_id);
    void signal_response(const string& message_id, message_type_t msg_type,
			 const string& msg);
    void check_response(message_type_t msg_type, const string& msg);
    
    template <class OBJ_T>
    typename OBJ_T::rop_t
    get_rop(const string& ws_url, const string& object_id) {
      auto ws = this->connect(ws_url, object_id);
      typename OBJ_T::rop_t ret(this, ws, ws_url, object_id);
      return ret;
    }

    template <class OBJ_T>
    typename OBJ_T::rop_t
    add_object(shared_ptr<Object> o, const string& object_id = "")
    {
      string real_object_id = this->add_object(o, object_id);
      typename OBJ_T::rop_t ret(this, real_object_id);
      return ret;
    }
  };
};

#endif

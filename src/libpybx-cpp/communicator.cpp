#include <iostream>
#include <sstream>
using namespace std;

#include <unistd.h> // sleep

#include <kvan/uuid.h>
#include <kvan/json-io.h>
#include <libpybx-cpp/communicator.h>
#include <libpybx-cpp/proto.h>
#include <libpybx-cpp/remote-methods.h>
#include <libpybx-cpp/exception.h>

void pybx::ws_send(shared_ptr<ix::WebSocket> ws, const string& msg)
{
  auto send_ret = ws->send(msg);
  if (send_ret.success == false) {
    cout << "send failed: " << ws << endl;
    throw BadROP("pybx::ws_send: send failed");
  }
}

pybx::Object::~Object()
{
}

pybx::Communicator::Communicator()
{
  RemoteMethods::set_communicator(this);
  worker_thread = move(thread(bind(&Communicator::do_dispatch_method_call_thread, this)));
  worker_thread.detach();
}

pybx::Communicator::~Communicator()
{
  cout << "pybx::Communicator::~Communicator()" << endl;
}

void pybx::Communicator::set_listen_port(int listen_port, const string& listen_host)
{
  this->listen_port = listen_port;
  this->listen_host = listen_host;
}

string pybx::Communicator::add_object(shared_ptr<Object> o,
					const string& object_id_)
{
  lock_guard g(objects_lock);
  string object_id = object_id_ == "" ? uuid::generate_uuid_v4() : object_id_;
  auto it = objects.find(object_id);
  if (it != objects.end()) {
    ostringstream m;
    m << "Communicator::add_object: object_id " << object_id << " was already taken";
    throw runtime_error(m.str());
  }
  objects[object_id] = o;
  return object_id;
}

shared_ptr<pybx::Object>
pybx::Communicator::find_object(const string& object_id)
{
  lock_guard g(objects_lock);
  auto it = objects.find(object_id);
  if (it == objects.end()) {
    ostringstream m;
    m << "pybx::Communicator::find_object: can't find object " << object_id;
    throw runtime_error(m.str());
  }
  return (*it).second;
}
    
shared_ptr<ix::WebSocket>
pybx::Communicator::connect(const string& ws_url, const string& object_id)
{
  auto webSocket = make_shared<ix::WebSocket>();
  webSocket->disableAutomaticReconnection();
  webSocket->setUrl(ws_url);
  webSocket->setOnMessageCallback([this, webSocket](const ix::WebSocketMessagePtr& msg) {
      cerr << "got something " << msg->str.size() << endl;
      if (msg->type == ix::WebSocketMessageType::Message)
        {
	  this->dispatch(webSocket, msg->str);
        }
    });

  // Now that our callback is setup, we can start our background thread and receive messages
  webSocket->start();
  while (webSocket->getReadyState() != ix::ReadyState::Open) {
    cerr << "in progress..." << endl;
    sleep(2);
  }

  return webSocket;
}

void pybx::Communicator::do_dispatch_method_call_thread()
{
  while (true) {
    try {
      tuple<shared_ptr<ix::WebSocket>, string, bool> m;
      cout << "pybx::Communicator::do_dispath_method_call_thread: start waiting for new message" << endl;
      worker_q.blocking_get(&m);
      auto [ws, msg, oneway_f] = m;
      cout << "pybx::Communicator::do_dispath_method_call_thread: msg: " << msg << endl;
      this->dispatch_method_call(ws, msg, oneway_f);
    } catch (exception& ex) {
      cout << "pybx::Communicator::do_dispatch_method_call_thread caught exception" << endl;
      cout << ex.what() << endl;
      break;
    }
  }
}

void pybx::Communicator::dispatch(shared_ptr<ix::WebSocket> ws, const string& msg)
{
  auto msg_type = get_message_type(msg);
  switch (msg_type) {
  case message_type_t::METHOD_CALL:
  case message_type_t::METHOD_ONEWAY_CALL:
    worker_q.blocking_put(make_tuple(ws, msg, msg_type == message_type_t::METHOD_ONEWAY_CALL));
    break;
  case message_type_t::METHOD_CALL_RETURN:
  case message_type_t::METHOD_CALL_EXCEPTION:
    dispatch_response(msg_type, msg);
    break;
  }
}

void pybx::Communicator::dispatch_method_call(shared_ptr<ix::WebSocket> ws,
					      const string& msg,
					      bool oneway_f)
{
  string method_signature = get_method_signature(msg);
  auto method_call = RemoteMethods::find_method(method_signature);
  string res_msg;
  method_call->do_call(msg, &res_msg, ws);
  if (oneway_f == false) {
    pybx::ws_send(ws, res_msg);
    cout << "pybx::Communicator::dispatch_method_call response: " << res_msg << endl;
  } else {
    cout << "pybx::Communicator::dispatch_method_call oneway call" << endl;
  }
}

void pybx::Communicator::dispatch_response(message_type_t msg_type,
					     const string& msg)
{
  auto orig_message_id = get_orig_message_id(msg);
  this->signal_response(orig_message_id, msg_type, msg);
}

void
pybx::Communicator::send_oneway(shared_ptr<ix::WebSocket> ws,
				const string& req_s,
				const string& message_id)
{  
  ws_send(ws, req_s);
}

pair<pybx::message_type_t, string>
pybx::Communicator::send_and_wait_for_response(shared_ptr<ix::WebSocket> ws,
					       const string& req_s,
					       const string& message_id)
{  
  shared_ptr<Waiter> w;
  {
    lock_guard g(waiters_lock);
    auto it = waiters.find(message_id);
    if (it != waiters.end()) {
      ostringstream m;
      m << "pybx::Communicator::wait_for_response: message_id is already waited for: " << message_id;
      throw runtime_error(m.str());
    }
    w = waiters[message_id] = make_shared<Waiter>();
  }

  ws_send(ws, req_s);
  
  pair<message_type_t, string> ret;
  w->blocking_get(&ret);

  {
    lock_guard g(waiters_lock);
    //cout << "erasing " << message_id << endl;
    waiters.erase(message_id);
  }
  
  return ret;
}

void pybx::Communicator::signal_response(const string& message_id,
					   message_type_t msg_type,
					   const string& msg)
{
  lock_guard g(waiters_lock);
  shared_ptr<Waiter> w;

  auto it = waiters.find(message_id);
  if (it == waiters.end()) {
    ostringstream m;
    m << "pybx::Communicator::signal_response: unknown message_id: " << message_id;
    throw runtime_error(m.str());
  }
  w = (*it).second;
  w->nonblocking_put(make_pair(msg_type, msg));
}

void pybx::Communicator::check_response(message_type_t msg_type,
					  const string& msg)
{
  switch (msg_type) {
  case pybx::message_type_t::METHOD_CALL_RETURN:
    break;
  case pybx::message_type_t::METHOD_CALL_EXCEPTION:
    {
      pybx::ExceptionResponse eres;
      from_json(&eres, msg);
      throw pybx::RemoteException(eres.remote_exception_text);
    }
    break;
  case pybx::message_type_t::METHOD_CALL:
  case pybx::message_type_t::METHOD_ONEWAY_CALL:
    throw runtime_error("pybx::Communicator::check_response: unexcepted message type METHOD_CALL");
    break;
  }
}

void pybx::Communicator::run()
{
  ix::WebSocketServer server(listen_port, listen_host);
  server.setOnConnectionCallback([&server, this]
				 (std::shared_ptr<ix::WebSocket> webSocket,
				  std::shared_ptr<ix::ConnectionState> connectionState) {
				   //std::cout << "Remote ip: " << connectionInfo->remoteIp << std::endl;
				   webSocket->disableAutomaticReconnection();
				   webSocket->setOnMessageCallback([webSocket, connectionState, &server, this](const ix::WebSocketMessagePtr& msg) {
				       if (msg->type == ix::WebSocketMessageType::Open) {
					 std::cout << "New connection" << std::endl;
					 
					 // A connection state object is available, and has a default id
					 // You can subclass ConnectionState and pass an alternate factory
					 // to override it. It is useful if you want to store custom
					 // attributes per connection (authenticated bool flag, attributes, etc...)
					 std::cout << "id: " << connectionState->getId() << std::endl;
					 
					 // The uri the client did connect to.
					 std::cout << "Uri: " << msg->openInfo.uri << std::endl;
					 
					 std::cout << "Headers:" << std::endl;
					 for (auto it : msg->openInfo.headers) {
					   std::cout << it.first << ": " << it.second << std::endl;
					 }
				       } else if (msg->type == ix::WebSocketMessageType::Message) {
					 string res_s;
					 //cout << "message " << msg->str.size() << endl;
					 this->dispatch(webSocket, msg->str);
				       } else if (msg->type == ix::WebSocketMessageType::Close) {
					 cout << "connection closed" << endl;
				       } else if (msg->type == ix::WebSocketMessageType::Ping) {
					 cout << __func__ << ": received ping" << endl;
				       } else {
					 ostringstream m;
					 m << __func__ << ": unknown message " << int(msg->type);
					 throw runtime_error(m.str());
				       }
				     });				     
				 });
  
  auto res = server.listen();
  if (!res.first) {
    // error
    throw runtime_error("main: can't listen");
  }
  
  server.start();
  server.wait();    
}


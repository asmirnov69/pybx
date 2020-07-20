#include <iostream>
#include <sstream>
using namespace std;

#include <unistd.h> // sleep

#include <libdipole/communicator.h>
#include <libdipole/proto.h>

Dipole::Communicator::Communicator()
{
  Methods::set_comminicator(this);
}

void Dipole::Communicator::set_listen_port(int listen_port)
{
  this->listen_port = listen_port;
}

string Dipole::Communicator::add_object(shared_ptr<Object> o, const string& object_id)
{
  auto it = objects.find(object_id);
  if (it != objects.end()) {
    ostringstream m;
    m << "Communicator::add_object: object_id " << object_id << " was already taken";
    throw runtime_error(m.str());
  }
  objects[object_id] = o;
  return object_id;
}

Dipole::ObjectPtr Dipole::Communicator::get_object_ptr(const string& object_id)
{
  ObjectPtr ret;
  //ret.ws_url = ...;
  ret.object_id = object_id;
  return ret;
}

Dipole::ObjectPtr Dipole::Communicator::connect(const string& ws_url, const string& object_id)
{
  auto webSocket = make_shared<ix::WebSocket>();
  webSocket->setUrl(ws_url);
  webSocket->setOnMessageCallback([this, webSocket](const ix::WebSocketMessagePtr& msg) {
      cerr << "got something" << endl;
      if (msg->type == ix::WebSocketMessageType::Message)
        {
	  std::cout << msg->str << std::endl;
	  this->dispatch(webSocket, msg->str);
        }
    });

  // Now that our callback is setup, we can start our background thread and receive messages
  webSocket->start();
  while (webSocket->getReadyState() != ix::ReadyState::Open) {
    cerr << "in progress..." << endl;
    sleep(2);
  }

  ObjectPtr ret;
  ret.ws = webSocket;
  ret.ws_url = ws_url;
  ret.object_id = object_id;
  return ret;
}

void Dipole::Communicator::dispatch(shared_ptr<ix::WebSocket> ws, const string& msg)
{
  switch (get_message_type(msg)) {
  case message_type_t::METHOD_CALL:
    dispatch_method_call(ws, msg);
    break;
  case message_type_t::METHOD_CALL_RETURN:
    dispatch_method_call_return(msg);
    break;
  }
}

void Dipole::Communicator::dispatch_method_call(shared_ptr<ix::WebSocket>  ws, const string& msg)
{
  string method_signature = get_method_signature(msg);
  auto method_call = Methods::find_method(method_signature);
  string res_msg;
  method_call->do_call(msg, &res_msg);
  ws->sendBinary(res_msg);
}

void Dipole::Communicator::dispatch_method_call_return(const string& msg)
{
  int orig_message_id = get_orig_message_id(msg);
  this->signal_response(orig_message_id, msg);
}

void Dipole::Communicator::run()
{
  ix::WebSocketServer server(listen_port);
  server.setOnConnectionCallback([&server, this]
				 (std::shared_ptr<ix::WebSocket> webSocket,
				  std::shared_ptr<ix::ConnectionState> connectionState) {
				   //std::cout << "Remote ip: " << connectionInfo->remoteIp << std::endl;
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
					 cout << "message: " << msg->str << endl;
					 this->dispatch(webSocket, msg->str);
				       } else {
					 throw runtime_error("handle_ws_messages: unknown message");
				       }
				     });				     
				 });
  
  auto res = server.listen();
  if (!res.first) {
    // error
    throw runtime_error("main: can't listen");
  }
  
  cout << "server start" << endl;
  server.start();
  server.wait();    
}


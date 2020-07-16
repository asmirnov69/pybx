#include <iostream>
using namespace std;

#include <ixwebsocket/IXWebSocketServer.h>
#include "dipole.h"
#include "backend_idl.h"
#include "HelloI.h"

int main()
{
  shared_ptr<ServerObjectManager> om = make_shared<ServerObjectManager>();
  shared_ptr<Hello> hello_o = make_shared<HelloI>();
  om->add_object("hello", hello_o);

  int port = 8080;
  ix::WebSocketServer server(port);
  server.setOnConnectionCallback([&server, om]
				 (std::shared_ptr<ix::WebSocket> webSocket,
				  std::shared_ptr<ix::ConnectionState> connectionState) {
				   //std::cout << "Remote ip: " << connectionInfo->remoteIp << std::endl;
				   webSocket->setOnMessageCallback([webSocket, connectionState, &server, om](const ix::WebSocketMessagePtr& msg)
                                   {
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
				       dispatch(om, msg->str, &res_s);
				       cout << "response: " << res_s << endl;
				       webSocket->send(res_s);
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

#include <iostream>
using namespace std;

#include <libdipole/communicator.h>
#include "backend_idl.h"
#include "HelloI.h"

int main()
{
  int port = 8080;
  Dipole::Communicator comm;
  comm.set_listen_port(port);
  
  shared_ptr<Hello> hello_o = make_shared<HelloI>();
  comm.add_object(hello_o, "hello");
  auto hello_o_ptr = comm.get_object_ptr("hello");
  
  cout << "server start" << endl;
  comm.run();
}

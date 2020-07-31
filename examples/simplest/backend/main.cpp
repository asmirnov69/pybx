#include <iostream>
using namespace std;

#include <libpybx-cpp/communicator.h>
#include "backend.h"
#include "HelloI.h"

int main()
{
  int port = 8080;
  Dipole::Communicator comm;
  comm.set_listen_port(port);
  
  shared_ptr<Hello> hello_o = make_shared<HelloI>();
  auto hello_ptr = comm.add_object<Hello>(hello_o, "hello");
  
  cout << "server start" << endl;
  comm.run();
}

#include <iostream>
using namespace std;

#include <libpybx-cpp/communicator.h>
#include "backend_pybx.h"
#include "HelloI.h"

int main()
{
  string host = "0.0.0.0";
  int port = 12345;
  pybx::Communicator comm;
  comm.set_listen_port(port, host);
  
  shared_ptr<pybx::backend::Hello> hello_o = make_shared<HelloI>();
  auto hello_rop = comm.add_object<pybx::backend::Hello>(hello_o, "hello");
  
  cout << "server start" << endl;
  comm.run();
}

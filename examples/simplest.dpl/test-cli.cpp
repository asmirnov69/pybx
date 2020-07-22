#include <iostream>
using namespace std;

#include <libdipole/communicator.h>
#include "backend_idl.h"

int main() {
  Dipole::Communicator comm;
  HelloPtr hello_ptr = comm.get_ptr<Hello>("ws://localhost:8080/", "hello");

  cout << "start" << endl;
  for (int i = 0; i < 5; i++) {
    Greetings g = hello_ptr->sayHello("hi");
    cout << "got back: " << endl;
    cout << g.language << " " << g.text
	 << " " << get_enum_value_string(g.color)
	 << endl;
  }
  
  try {
    hello_ptr->sayHello("HI");
  } catch (Dipole::RemoteException& rex) {
    cout << "remote exception caught: " << rex.what() << endl;
  }

#if 0
  auto hellocb_o = make_shared<HelloCBImpl>();
  HelloCBPtr hellocb_ptr = comm.add_object(hellocb_o);
  hello_ptr->register_hello_cb(hellocb_ptr);
#endif

  Greetings gg = hello_ptr->sayHello("hi");
  cout << "got back: " << endl;
  cout << gg.language << " " << gg.text
	 << " " << get_enum_value_string(gg.color)
	 << endl;
  
  return 0;
}

#include <iostream>
using namespace std;

#include <kvan/fuargs.h>
#include <libdipole-cpp/communicator.h>
#include "backend.h"
#include "HelloI.h"

ADD_ACTION("test_call[]", [](const Fuargs::args&) {
    Dipole::Communicator comm;
    HelloPtr hello_ptr = comm.get_ptr<Hello>("ws://localhost:8080/", "hello");
    
    cout << "start" << endl;
    for (int i = 0; i < 5; i++) {
      Greetings g = hello_ptr.sayHello("hi");
      cout << "got back: " << endl;
      cout << g.language << " " << g.text
	   << " " << get_enum_value_string(g.color)
	   << endl;
    }

    GreetingsSeq gs;
    gs.push_back(Greetings{.language = "russian", .text = "privet", .color = Color::GREEN});
    gs.push_back(Greetings{.language = "german", .text = "halo", .color = Color::GREEN});

    auto new_gs = hello_ptr.reformatGreetings(gs);
    cout << "got " << new_gs.size() << " greetings" << endl;
    
    try {
      Greetings gg = hello_ptr.sayHello("HI");
    } catch (Dipole::RemoteException& rex) {
      cout << "remote exception caught: " << rex.what() << endl;
    }

    return true;
  });

ADD_ACTION("test_cb[]", [](const Fuargs::args&) {
    Dipole::Communicator comm;
    HelloPtr hello_ptr = comm.get_ptr<Hello>("ws://localhost:8080/", "hello");

    auto hellocb_o = make_shared<HelloCBI>();
    HelloCBPtr hellocb_ptr = comm.add_object<HelloCB>(hellocb_o);
    hello_ptr.register_hello_cb(hellocb_ptr);

    Greetings gg = hello_ptr.sayHello("hi");
    cout << "got back: " << endl;
    cout << gg.language << " " << gg.text
	 << " " << get_enum_value_string(gg.color)
	 << endl;
    
    return true;
  });

int main(int argc, char** argv)
{
  Fuargs::exec_actions(argc, argv);
}


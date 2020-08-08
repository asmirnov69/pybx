#include <iostream>
using namespace std;

#include <kvan/fuargs.h>
#include <libpybx-cpp/communicator.h>
#include "backend.h"
#include "HelloI.h"

ADD_ACTION("test_call[]", [](const Fuargs::args&) {
    pybx::Communicator comm;
    auto hello_ptr = comm.get_ptr<backend::Hello>("ws://localhost:8080/", "hello");
    
    cout << "start" << endl;
    for (int i = 0; i < 5; i++) {
      auto g = hello_ptr.sayHello("hi");
      cout << "got back: " << endl;
      cout << g.language << " " << g.text
	   << " " << get_enum_value_string(g.color)
	   << endl;
    }

    vector<backend::Greetings> gs;
    gs.push_back(backend::Greetings{.language = "russian", .text = "privet", .color = backend::Color::GREEN});
    gs.push_back(backend::Greetings{.language = "german", .text = "halo", .color = backend::Color::GREEN});

    auto new_gs = hello_ptr.reformatGreetings(gs);
    cout << "got " << new_gs.size() << " greetings" << endl;
    
    try {
      auto gg = hello_ptr.sayHello("HI");
    } catch (pybx::RemoteException& rex) {
      cout << "remote exception caught: " << rex.what() << endl;
    }

    return true;
  });

ADD_ACTION("test_cb[]", [](const Fuargs::args&) {
    pybx::Communicator comm;
    auto hello_ptr = comm.get_ptr<backend::Hello>("ws://localhost:8080/", "hello");

    auto hellocb_o = make_shared<HelloCBI>();
    auto hellocb_ptr = comm.add_object<backend::HelloCB>(hellocb_o);
    hello_ptr.register_hello_cb(hellocb_ptr);

    auto gg = hello_ptr.sayHello("hi");
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


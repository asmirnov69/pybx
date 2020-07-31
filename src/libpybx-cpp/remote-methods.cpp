#include <iostream>
#include <libpybx-cpp/remote-methods.h>

map<string, shared_ptr<Dipole::method_impl>> Dipole::RemoteMethods::remote_methods;

int Dipole::RemoteMethods::register_method(const string& method_signature,
					   shared_ptr<method_impl> mi)
{
  cout << "Dipole::RemoteMethods::register_method: " << method_signature << endl;
  auto it = remote_methods.find(method_signature);
  if (it == remote_methods.end()) {
    remote_methods[method_signature] = mi;
  }
  return 1;
}

shared_ptr<Dipole::method_impl>
Dipole::RemoteMethods::find_method(const string& method_signature)
{
  auto it = remote_methods.find(method_signature);
  if (it == remote_methods.end()) {
    ostringstream m;
    m << "Dipole::RemoteMethods::find_method: can't find method " << method_signature;
    throw runtime_error(m.str());
  }
  return (*it).second;
}

void Dipole::RemoteMethods::set_communicator(Communicator* comm)
{
  for (auto it = remote_methods.begin(); it != remote_methods.end(); ++it) {
    (*it).second->comm = comm;
  }
}

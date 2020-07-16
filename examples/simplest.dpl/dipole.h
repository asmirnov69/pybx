// -*- c++ -*-
#ifndef __DIPOLE_HH__
#define __DIPOLE_HH__

#include <memory>
#include <string>
#include <map>
using namespace std;

class ServerObject
{
public:
  virtual ~ServerObject() = 0;
};

class ServerObjectManager
{
private:
  map<string, shared_ptr<ServerObject>> objects;
  
public:
  shared_ptr<ServerObject> lookup(const string& object_id);
  void add_object(const string& object_id, shared_ptr<ServerObject>);
};

#endif

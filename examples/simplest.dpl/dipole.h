// -*- c++ -*-
#ifndef __DIPOLE_HH__
#define __DIPOLE_HH__

#include <memory>
#include <string>
using namespace std;

class ServerObject
{
public:
  virtual ~ServerObject() = 0;
};

class ServerObjectManager
{
public:
  shared_ptr<ServerObject> lookup(const string& object_id);
  void add_object(const string& object_id, shared_ptr<ServerObject>);
};

#endif

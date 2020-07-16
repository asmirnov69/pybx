#include <sstream>
using namespace std;

#include "dipole.h"

ServerObject::~ServerObject()
{
}

shared_ptr<ServerObject> ServerObjectManager::lookup(const string& object_id)
{
  shared_ptr<ServerObject> ret;
  auto it = objects.find(object_id);
  if (it != objects.end()) {
    ret = (*it).second;
  }
  return ret;
}

void ServerObjectManager::add_object(const string& object_id, shared_ptr<ServerObject> o) {
  auto it = objects.find(object_id);
  if (it != objects.end()) {
    ostringstream m;
    m << "ServerObjectManager::add_object: object_id " << object_id << " was already taken";
    throw runtime_error(m.str());
  }
  objects[object_id] = o;
}

// -*- c++ -*-
// generated code: source - ./Blotter.pybx
#ifndef __BLOTTER_STUBS_HH__
#define __BLOTTER_STUBS_HH__

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE static int TOKENPASTE2(Unique_, __LINE__)

#include <memory>
#include <string>
#include <stdexcept>
using namespace std;

#include <kvan/json-io.h>
#include <libpybx-cpp/communicator.h>
#include <libpybx-cpp/proto.h>
#include <libpybx-cpp/remote-methods.h>
    
namespace Blotter {
struct DataFrame {
 vector<string> columns;
 string dataframeJSON;
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DataFrame>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("columns", &Blotter::DataFrame::columns),
  make_member_descriptor("dataframeJSON", &Blotter::DataFrame::dataframeJSON),
 };
 return sd;
}
namespace Blotter {
struct DFWUPC {
 Blotter::DataFrame df;
 int update_c;
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DFWUPC>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("df", &Blotter::DFWUPC::df),
  make_member_descriptor("update_c", &Blotter::DFWUPC::update_c),
 };
 return sd;
}
namespace Blotter {
class DFTest_rop;
class Observer_rop;
}
namespace Blotter {
class DFTest_rop {
private:
  pybx::Communicator* comm{nullptr};
  std::shared_ptr<ix::WebSocket> ws;
public:
  std::string object_id;
  std::string __interface_type{"Blotter.DFTest_rop"};
  DFTest_rop();
  DFTest_rop(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);
  DFTest_rop(pybx::Communicator* comm, const std::string& object_id);
  void activate(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);
  Blotter::DFWUPC get_df();
  void subscribe(Blotter::Observer_rop rop);
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DFTest_rop>()
{
 static const StructDescriptor sd = {
   make_member_descriptor("object_id", &Blotter::DFTest_rop::object_id),
   make_member_descriptor("__interface_type", &Blotter::DFTest_rop::__interface_type),
 };
 return sd;
}
namespace Blotter {
class Observer_rop {
private:
  pybx::Communicator* comm{nullptr};
  std::shared_ptr<ix::WebSocket> ws;
public:
  std::string object_id;
  std::string __interface_type{"Blotter.Observer_rop"};
  Observer_rop();
  Observer_rop(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws, const std::string& ws_url, const std::string& object_id);
  Observer_rop(pybx::Communicator* comm, const std::string& object_id);
  void activate(pybx::Communicator* comm, std::shared_ptr<ix::WebSocket> ws);
  void show(Blotter::DFWUPC df);
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::Observer_rop>()
{
 static const StructDescriptor sd = {
   make_member_descriptor("object_id", &Blotter::Observer_rop::object_id),
   make_member_descriptor("__interface_type", &Blotter::Observer_rop::__interface_type),
 };
 return sd;
}
namespace Blotter {
class DFTest : public pybx::Object {
public:
 typedef DFTest_rop rop_t;
 virtual Blotter::DFWUPC get_df() = 0;
 virtual void subscribe(Blotter::Observer_rop rop) = 0;
};
}
namespace Blotter {
struct DFTest__get_df : public pybx::method_impl
{
 struct args_t {
 
 };
 struct return_t {
   Blotter::DFWUPC retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DFTest__get_df::args_t>()
{
 static const StructDescriptor sd = {
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<Blotter::DFTest__get_df::args_t>>()
{
 return get_StructDescriptor_T<Blotter::DFTest__get_df::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DFTest__get_df::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &Blotter::DFTest__get_df::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<Blotter::DFTest__get_df::return_t>>()
{
 return get_StructDescriptor_T<Blotter::DFTest__get_df::return_t, pybx::Response>::get_struct_descriptor();
}
namespace Blotter {
struct DFTest__subscribe : public pybx::method_impl
{
 struct args_t {
 Blotter::Observer_rop rop;
 };
 struct return_t {
   json_null_t retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DFTest__subscribe::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("rop", &Blotter::DFTest__subscribe::args_t::rop),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<Blotter::DFTest__subscribe::args_t>>()
{
 return get_StructDescriptor_T<Blotter::DFTest__subscribe::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::DFTest__subscribe::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &Blotter::DFTest__subscribe::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<Blotter::DFTest__subscribe::return_t>>()
{
 return get_StructDescriptor_T<Blotter::DFTest__subscribe::return_t, pybx::Response>::get_struct_descriptor();
}
namespace Blotter {
class Observer : public pybx::Object {
public:
 typedef Observer_rop rop_t;
 virtual void show(Blotter::DFWUPC df) = 0;
};
}
namespace Blotter {
struct Observer__show : public pybx::method_impl
{
 struct args_t {
 Blotter::DFWUPC df;
 };
 struct return_t {
   json_null_t retval;
 };
 void do_call(const std::string& req_s, std::string* res_s, std::shared_ptr<ix::WebSocket>) override;
};
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::Observer__show::args_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("df", &Blotter::Observer__show::args_t::df),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Request<Blotter::Observer__show::args_t>>()
{
 return get_StructDescriptor_T<Blotter::Observer__show::args_t, pybx::Request>::get_struct_descriptor();
}
template <> inline StructDescriptor get_struct_descriptor<Blotter::Observer__show::return_t>()
{
 static const StructDescriptor sd = {
  make_member_descriptor("retval", &Blotter::Observer__show::return_t::retval),
 };
 return sd;
}
template <> inline StructDescriptor get_struct_descriptor<pybx::Response<Blotter::Observer__show::return_t>>()
{
 return get_StructDescriptor_T<Blotter::Observer__show::return_t, pybx::Response>::get_struct_descriptor();
}
namespace Blotter {
inline DFTest_rop::DFTest_rop()
{
}
inline DFTest_rop::DFTest_rop(pybx::Communicator* comm,
std::shared_ptr<ix::WebSocket> ws,
const std::string& ws_url, const std::string& object_id)
{
 this->comm = comm;
 this->ws = ws;
 this->object_id = object_id;
}
inline DFTest_rop::DFTest_rop(pybx::Communicator* comm, const std::string& object_id)
{
 this->comm = comm;
 this->object_id = object_id;
}
inline void DFTest_rop::activate(pybx::Communicator* c, std::shared_ptr<ix::WebSocket> ws)

    {
      this->comm = c;
      if (this->ws == nullptr) {
        this->ws = ws;
      } else {
          throw runtime_error("DFTest_rop::activate: not implemented for universal rop");
      }
    }
    
inline Blotter::DFWUPC DFTest_rop::get_df()
{

    pybx::Request<DFTest__get_df::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "DFTest__get_df",
      .object_id = object_id,
      .args = DFTest__get_df::args_t()
      };

    ;
     Blotter::DFWUPC ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<DFTest__get_df::return_t> res;
    from_json(&res, res_s.second);
     ret = res.retval.retval;
     return ret;
    
}
inline void DFTest_rop::subscribe(Blotter::Observer_rop rop)
{
    pybx::Request<DFTest__subscribe::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "DFTest__subscribe",
      .object_id = object_id,
      .args = DFTest__subscribe::args_t()
      };

    req.args.rop=rop;
    // void ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<DFTest__subscribe::return_t> res;
    from_json(&res, res_s.second);
    // ret = res.retval.retval;
    // return ret;
    
}
} // end of namespace
namespace Blotter {
inline Observer_rop::Observer_rop()
{
}
inline Observer_rop::Observer_rop(pybx::Communicator* comm,
std::shared_ptr<ix::WebSocket> ws,
const std::string& ws_url, const std::string& object_id)
{
 this->comm = comm;
 this->ws = ws;
 this->object_id = object_id;
}
inline Observer_rop::Observer_rop(pybx::Communicator* comm, const std::string& object_id)
{
 this->comm = comm;
 this->object_id = object_id;
}
inline void Observer_rop::activate(pybx::Communicator* c, std::shared_ptr<ix::WebSocket> ws)

    {
      this->comm = c;
      if (this->ws == nullptr) {
        this->ws = ws;
      } else {
          throw runtime_error("Observer_rop::activate: not implemented for universal rop");
      }
    }
    
inline void Observer_rop::show(Blotter::DFWUPC df)
{

    pybx::Request<Observer__show::args_t> req{
    .message_type = pybx::message_type_t::METHOD_CALL,
      .message_id = pybx::create_new_message_id(),
      .method_signature = "Observer__show",
      .object_id = object_id,
      .args = Observer__show::args_t()
      };

    req.args.df=df;
    // void ret;
  
    ostringstream json_os;
    to_json(json_os, req);  
    pybx::ws_send(ws, json_os.str());
    auto res_s = comm->wait_for_response(req.message_id);
    comm->check_response(res_s.first, res_s.second);
    
    pybx::Response<Observer__show::return_t> res;
    from_json(&res, res_s.second);
    // ret = res.retval.retval;
    // return ret;
    
}
} // end of namespace
namespace Blotter {
inline void DFTest__get_df::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<DFTest>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
       res.retval.retval = 
            self->get_df();
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("DFTest__get_df", std::make_shared<DFTest__get_df>());
inline void DFTest__subscribe::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{

    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      req.args.rop.activate(comm, ws);

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<DFTest>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
      // res.retval.retval = 
            self->subscribe(req.args.rop);
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("DFTest__subscribe", std::make_shared<DFTest__subscribe>());
}
namespace Blotter {
inline void Observer__show::do_call(const string& req_s, string* res_s, shared_ptr<ix::WebSocket> ws)
{
    ostringstream res_os;
    try {
      pybx::Request<args_t> req;
      from_json(&req, req_s);

      

      auto o = comm->find_object(req.object_id);
      auto self = dynamic_pointer_cast<Observer>(o);
      if (self == nullptr) {
        throw runtime_error("dyn type mismatch");
      }

      pybx::Response<return_t> res;
      res.message_id = pybx::create_new_message_id();
      res.orig_message_id = req.message_id;
      // res.retval.retval = 
            self->show(req.args.df);
      to_json(res_os, res);
    } catch (exception& e) {
      pybx::ExceptionResponse eres;
      eres.message_id = pybx::create_new_message_id();
      eres.orig_message_id = pybx::get_message_id(req_s);
      eres.remote_exception_text = e.what();
      to_json(res_os, eres);
    }

    *res_s = res_os.str();
    
}
UNIQUE = pybx::RemoteMethods::register_method("Observer__show", std::make_shared<Observer__show>());
}
#endif

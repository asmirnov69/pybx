#include <iostream>
#include <thread>
using namespace std;

#include <kvan/fuargs.h>
#include <kvan/fjson-io.h>
#include <libpybx-cpp/communicator.h>
#include "Blotter_pybx.h"
#include <unistd.h>

class ObserverI : public pybx::Blotter::Observer {
public:
  void show(pybx::Blotter::DFWUPC df) override {
    cout << "show: " << df.df.dataframeJSON << endl;
  }
};

class DFTestI : public pybx::Blotter::DFTest {
private:
  mutex obj_lock;
  vector<pybx::Blotter::Observer_rop> rops;
  
public:
  struct Rec {
    string city, state;
    double chance_of_rain{0.0};
  };
  vector<Rec> v;
  
  DFTestI() {
    this->v = vector<Rec>{
      Rec{.city = "Ashland", .state = "MA", .chance_of_rain = 0.2},
      Rec{.city = "Hopkinton", .state = "MA", .chance_of_rain = 0.23},
      Rec{.city = "Hartford", .state = "CT", .chance_of_rain = 0.22},
      Rec{.city = "Greenwich", .state = "CT", .chance_of_rain = 0.26}
    };
  }
  
  pybx::Blotter::DFWUPC get_df() override {
    lock_guard<mutex> l(obj_lock);
    ostringstream out;
    auto columns = to_fjson(out, v);
    pybx::Blotter::DataFrame df{.columns = columns, .dataframeJSON = out.str()};
    return pybx::Blotter::DFWUPC{.df = df, .update_c = -1};
  }

  pybx::Blotter::Observer_rop subscribe(pybx::Blotter::Observer_rop rop) override {
    cout << "DFTestI::subscribe" << endl;
    lock_guard<mutex> l(obj_lock);
    rops.push_back(rop);
    return rop;
  }

  void update_thread()
  {
    cout << "starting update thread" << endl;
    int i = 0;
    while (1) {
      cout << "update thread " << i++ << endl;
      {
	lock_guard<mutex> l(obj_lock);
	for (auto& el: v) {
	  el.chance_of_rain += 0.001;
	}
      }
      this->publish();
      sleep(1);
    }
  }
  
  void publish()
  {
    lock_guard<mutex> l(obj_lock);	
    ostringstream out;
    auto columns = to_fjson(out, v);
    pybx::Blotter::DataFrame df{.columns = columns, .dataframeJSON = out.str()};
    pybx::Blotter::DFWUPC dfwupc{.df = df, .update_c = -2};
    for (auto it = rops.begin(); it != rops.end(); ) {
      try {
	(*it).oneway = true;
	(*it).show(dfwupc);
	it++;
      } catch (pybx::BadROP& e) {
	cout << "caught pybx::BadROP: " << e.what() << endl;
	cout << "removing subscriber at pos " << (it - rops.begin()) << endl;
	it = rops.erase(it);
	cout << "new rops size: " << rops.size() << endl;
      }
    }
  }
};

template <> inline StructDescriptor get_struct_descriptor<DFTestI::Rec>()
{
  static const StructDescriptor sd{
    make_member_descriptor("city", &DFTestI::Rec::city),
      make_member_descriptor("state", &DFTestI::Rec::state),
      make_member_descriptor("chance_of_rain", &DFTestI::Rec::chance_of_rain)
      };
  return sd;
} 

ADD_ACTION("test_subscriber[]", [](const Fuargs::args&) {
    pybx::Communicator comm;
    auto testdf_rop = comm.get_rop<pybx::Blotter::DFTest>("ws://localhost:8080/", "test_df");
    auto subscriber_o = make_shared<ObserverI>();
    auto subscriber_rop = comm.add_object<pybx::Blotter::Observer>(subscriber_o, "test_df");
    testdf_rop.subscribe(subscriber_rop);
    comm.run();
    return true;
  });

ADD_ACTION("test[]", [](const Fuargs::args&) {
    pybx::Communicator comm;
    auto testdf_rop = comm.get_rop<pybx::Blotter::DFTest>("ws://localhost:8080/", "test_df");
    auto df = testdf_rop.get_df();
    cout << "df: " << df.df.dataframeJSON << endl;
    return true;
  });

ADD_ACTION("run[]", [](const Fuargs::args&) {
    int port = 8080;
    pybx::Communicator comm;
    comm.set_listen_port(port);

    auto dftest_o = make_shared<DFTestI>();
    thread t(bind(&DFTestI::update_thread, dftest_o));

    comm.add_object<pybx::Blotter::DFTest>(dftest_o, "test_df");    
    comm.run();

    return true;
  });

int main(int argc, char** argv)
{
  Fuargs::exec_actions(argc, argv, false, false);
}

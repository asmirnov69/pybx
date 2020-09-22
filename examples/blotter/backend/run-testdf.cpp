#include <iostream>
#include <thread>
using namespace std;

#include <kvan/fuargs.h>
#include <kvan/fjson-io.h>
#include <libpybx-cpp/communicator.h>
#include "Blotter.h"
#include <unistd.h>

class ObserverI : public Blotter::Observer {
public:
  void show(Blotter::DFWUPC df) override {
    cout << "show: " << df.df.dataframeJSON << endl;
  }
};

class DFTestI : public Blotter::DFTest {
private:
  mutex obj_lock;
  vector<Blotter::Observer_rop> rops;
  
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
  
  Blotter::DFWUPC get_df() override {
    lock_guard<mutex> l(obj_lock);
    ostringstream out;
    auto columns = to_fjson(out, v);
    Blotter::DataFrame df{.columns = columns, .dataframeJSON = out.str()};
    return Blotter::DFWUPC{.df = df, .update_c = -1};
  }

  void subscribe(Blotter::Observer_rop rop) override {
    cout << "DFTestI::subscribe" << endl;
    lock_guard<mutex> l(obj_lock);
    rops.push_back(rop);
  }

  void update_thread()
  {
    cout << "starting update thread" << endl;
    while (1) {
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
    Blotter::DataFrame df{.columns = columns, .dataframeJSON = out.str()};
    Blotter::DFWUPC dfwupc{.df = df, .update_c = -2};
    for (auto& rop: rops) {
      rop.show(dfwupc);
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
    auto testdf_rop = comm.get_rop<Blotter::DFTest>("ws://localhost:8080/", "test_df");
    auto subscriber_o = make_shared<ObserverI>();
    auto subscriber_rop = comm.add_object<Blotter::Observer>(subscriber_o, "test_df");
    testdf_rop.subscribe(subscriber_rop);
    comm.run();
    return true;
  });

ADD_ACTION("test[]", [](const Fuargs::args&) {
    pybx::Communicator comm;
    auto testdf_rop = comm.get_rop<Blotter::DFTest>("ws://localhost:8080/", "test_df");
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

    comm.add_object<Blotter::DFTest>(dftest_o, "test_df");    
    comm.run();

    return true;
  });

int main(int argc, char** argv)
{
  Fuargs::exec_actions(argc, argv, false, false);
}

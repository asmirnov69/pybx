rm *.o *.tsk

export top_dir=/home/asmirnov/dipole
export IXWS_HOME=/home/asmirnov/3rdparty/IXWebSocket-9.6.4
export YAJL_HOME=/usr/local/philotymic/3rdparty

g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -I${top_dir}/src -c main.cpp
g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -I${top_dir}/src -c HelloI.cpp
g++ -g -Wall --std=c++17 -o server.tsk main.o HelloI.o -L${top_dir}/src/libdipole -llibdipole -L${KVAN_HOME}/cpp/src -lKVAN -L${YAJL_HOME}/lib -lyajl -L${IXWS_HOME}/build -lixwebsocket -pthread -lz -lssl -lcrypto -lstdc++fs -lyaml

#g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -I${top_dir}/src -c test-cli.cpp
#g++ -g -Wall --std=c++17 -o test-cli.tsk test-cli.o -L${top_dir}/src/libdipole -llibdipole -L${KVAN_HOME}/cpp/src -lKVAN -L${YAJL_HOME}/lib -lyajl -L${IXWS_HOME}/build -lixwebsocket -pthread -lz -lssl -lcrypto -lstdc++fs -lyaml

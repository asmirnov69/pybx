export IXWS_HOME=/home/asmirnov/3rdparty/IXWebSocket-9.6.4
export YAJL_HOME=/usr/local/philotymic/3rdparty
g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c main.cpp
g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c dipole.cpp
g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c HelloI.cpp
g++ -g -Wall --std=c++17 -o server.tsk main.o dipole.o HelloI.o -L${KVAN_HOME}/cpp/src -lKVAN -L${YAJL_HOME}/lib -lyajl -L${IXWS_HOME}/build -lixwebsocket -pthread -lz -lssl -lcrypto -lstdc++fs -lyaml

g++ -g -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c test-cli.cpp
g++ -g -Wall --std=c++17 -o test-cli.tsk test-cli.o -L${IXWS_HOME}/build -lixwebsocket -pthread -lz -lssl -lcrypto

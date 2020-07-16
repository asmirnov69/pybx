export IXWS_HOME=/home/asmirnov/3rdparty/IXWebSocket-9.6.4
#g++ -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c main.cpp
#g++ -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c HelloI.cpp
#g++ -Wall --std=c++17 -o server.tsk main.o HelloI.o -L${IXWS_HOME}/build -lixwebsocket -pthread -lz -lssl -lcrypto

g++ -Wall --std=c++17 -I${IXWS_HOME} -I${KVAN_HOME}/cpp/include -c test-cli.cpp
g++ -Wall --std=c++17 -o test-cli.tsk test-cli.o -L${IXWS_HOME}/build -lixwebsocket -pthread -lz -lssl -lcrypto


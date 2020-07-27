project(simplest.dpl-backend)

include_directories(${IXWS_HOME} ${KVAN_HOME}/cpp/include ${top}/src)
link_directories(${YAJL_HOME}/lib ${IXWS_HOME}/build ${KVAN_HOME}/cpp/src)

add_executable(server.tsk main.cpp HelloI.cpp)
target_link_libraries(server.tsk libdipole KVAN yaml yajl ixwebsocket pthread stdc++fs ssl crypto z)

add_executable(test-cli.tsk test-cli.cpp HelloI.cpp)
target_link_libraries(test-cli.tsk libdipole KVAN yaml yajl ixwebsocket pthread stdc++fs ssl crypto z)

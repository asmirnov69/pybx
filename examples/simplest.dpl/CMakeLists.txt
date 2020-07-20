project(simplest.dpl)

include_directories(${IXWS_HOME} ${KVAN_HOME}/cpp/include ${top}/src)
link_directories(${YAJL_HOME}/lib ${IXWS_HOME}/build ${KVAN_HOME}/cpp/src)

add_executable(server.tsk main.cpp)
target_link_libraries(server.tsk KVAN yaml yajl stdc++fs)

add_executable(test-cli.tsk test-cli.cpp)
target_link_libraries(test-cli.tsk KVAN yaml yajl stdc++fs)

project(simplest.dpl-backend)

include_directories(${IXWS_HOME} ${KVAN_HOME}/cpp/include ${top}/src)
link_directories(${IXWS_HOME}/build ${KVAN_HOME}/cpp/src)

add_executable(server.tsk main.cpp HelloI.cpp)
target_link_libraries(server.tsk libpybx-cpp kvan)

add_executable(test-cli.tsk test-cli.cpp HelloI.cpp)
target_link_libraries(test-cli.tsk libpybx-cpp kvan)

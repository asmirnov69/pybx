project(blotter)

include_directories(${IXWS_HOME} ${KVAN_HOME}/cpp/include ${top}/src)
link_directories(${YAJL_HOME}/lib ${IXWS_HOME}/build ${KVAN_HOME}/cpp/src)

add_executable(run-testdf.tsk run-testdf.cpp)
target_link_libraries(run-testdf.tsk libpybx-cpp KVAN yaml yajl ixwebsocket pthread stdc++fs ssl crypto z)


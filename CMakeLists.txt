project(pybx-all-projects)
exec_program(git ARGS "rev-parse --show-toplevel" OUTPUT_VARIABLE top)

set(IXWS_HOME /usr/local/3rd-party/IXWebSocket-9.6.4)
set(KVAN_HOME $ENV{KVAN_HOME})
set(PYBX_HOME $ENV{PYBX_HOME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20 -pthread -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-z,defs")

add_subdirectory(src)
add_subdirectory(examples)

project(pybx-all-projects)
exec_program(git ARGS "rev-parse --show-toplevel" OUTPUT_VARIABLE top)

set(YAJL_HOME /usr/local/philotymic/3rdparty)
set(IXWS_HOME /usr/local/philotymic/3rdparty/IXWebSocket-9.6.4)
set(KVAN_HOME $ENV{KVAN_HOME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall")

add_subdirectory(src)
add_subdirectory(examples)

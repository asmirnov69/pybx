project(blotter)
include(${top}/cmake/pybx-stubs.mak)

gen_pybx_cpp_stubs(gen_files ./Utils.pybx ./Blotter.pybx)

include_directories(${IXWS_HOME} ${KVAN_HOME}/cpp/include ${top}/src)
link_directories(${IXWS_HOME}/build ${KVAN_HOME}/cpp/src)

add_executable(run-testdf.tsk ${gen_files} run-testdf.cpp)
target_link_libraries(run-testdf.tsk libpybx-cpp kvan)


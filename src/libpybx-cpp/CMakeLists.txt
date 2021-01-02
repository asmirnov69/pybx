project(libpybx-cpp)

include_directories(${top}/src ${IXWS_HOME} ${KVAN_HOME}/cpp/include)

file(GLOB SRC_FILES *.cpp)
link_directories(${IXWS_HOME}/build ${KVAN_HOME}/cpp/src)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} kvan ixwebsocket ssl crypto z)

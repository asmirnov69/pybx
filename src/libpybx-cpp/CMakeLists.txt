project(libpybx-cpp)

include_directories(${top}/src ${IXWS_HOME} ${KVAN_HOME}/cpp/include)
link_directories(${IXWS_HOME}/build)

file(GLOB SRC_FILES *.cpp)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ixwebsocket ssl crypto z)

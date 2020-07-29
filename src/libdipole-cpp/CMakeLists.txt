project(libdipole-cpp)

include_directories(${top}/src ${YAJL_HOME}/include ${IXWS_HOME} ${KVAN_HOME}/cpp/include)
file(GLOB SRC_FILES *.cpp)
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

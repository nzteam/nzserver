cmake_minimum_required(VERSION 2.8)

project(nzcs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../dist/nzcs)

include_directories("${PROJECT_SOURCE_DIR}/../nzlib")
LINK_LIBRARIES(${LINK_LIBRARIES} ${PROJECT_SOURCE_DIR}/../dist/nzlib/libnzlib.a)

file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/*.h)

add_executable(nzcs ${source} ${headers})
target_link_libraries(nzcs mysqlcppconn)

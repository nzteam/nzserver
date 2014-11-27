cmake_minimum_required(VERSION 2.8)

project(nzgs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../dist/nzgs)

include_directories("${PROJECT_SOURCE_DIR}/../nzlib")
LINK_LIBRARIES(${LINK_LIBRARIES} ${PROJECT_SOURCE_DIR}/../dist/nzlib/libnzlib.a)

file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/*.h)

add_executable(nzgs ${source} ${headers})
target_link_libraries(nzgs mysqlcppconn)

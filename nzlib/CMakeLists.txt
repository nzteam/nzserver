cmake_minimum_required(VERSION 2.8)

project(nzlib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../dist/nzlib)

file(GLOB_RECURSE source ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/*.h)

add_library(nzlib ${source} ${headers})

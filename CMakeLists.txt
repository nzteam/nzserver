cmake_minimum_required(VERSION 2.8)

project(nzServer)

if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    MESSAGE(FATAL_ERROR "* Invalid environment! Should be Linux!")
endif()

find_package(Boost 1.45.0 COMPONENTS)

if(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "* Boost 1.45.0 >= not found")
else()
    INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})
    LINK_LIBRARIES(${LINK_LIBRARIES} ${Boost_LIBRARIES})
endif()

if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE DEBUG)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -g -Wall -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -pthread -O3")

add_subdirectory(nzlib)

add_subdirectory(nzcs)

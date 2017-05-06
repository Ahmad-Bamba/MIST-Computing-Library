cmake_minimum_required (VERSION 2.8)

project(Mist-Computing-Library CXX)
set(CMAKE_CXX_STANDARD $ENV{cpp_vers})
set(CMAKE_CXX_STANDARD_REQUIRED on)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
    include_directories(${CMAKE_SOURCE_DIR}/protobuf/src/)
endif(UNIX)

if(WIN32)
    include_directories("C:/Protobuf/protobuf/include/")
endif(WIN32)

include_directories(${CMAKE_SOURCE_DIR}/protobuf_files/)
# include_directories(${CMAKE_SOURCE_DIR}/asio-1.10.8/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)

# add_definitions(-DASIO_STANDALONE)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB_RECURSE Mist_Files ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE Protobuf_Files ${CMAKE_SOURCE_DIR}/protobuf_files/*.pb.cc)

add_library(MistComputingLibrary SHARED ${Mist_files} src/MIST.cpp)
add_library(MistComputingLibraryStatic STATIC ${Mist_files} src/MIST.cpp)

set_target_properties(MistComputingLibrary PROPERTIES OUTPUT_NAME Mist)
set_target_properties(MistComputingLibraryStatic PROPERTIES OUTPUT_NAME MistStatic)

set_target_properties(MistComputingLibrary PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(MistComputingLibraryStatic PROPERTIES LINKER_LANGUAGE CXX)

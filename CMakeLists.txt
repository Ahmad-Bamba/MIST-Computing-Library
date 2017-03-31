cmake_minimum_required (VERSION 3.0.2)

project(Mist-Computing-Library CXX)
set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/protobuf_files/)
include_directories(${CMAKE_SOURCE_DIR}/protobuf/src/)
include_directories(${CMAKE_SOURCE_DIR}/asio-1.10.8/include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)

file(GLOB_RECURSE Mist_Files ${CMAKE_SOURCE_DIR}/include/*.hpp)

add_library(MistComputingLibrary SHARED ${Mist_files} src/MIST.cpp)
add_library(MistComputingLibraryStatic STATIC ${Mist_files} src/MIST.cpp)

set_target_properties(MistComputingLibrary PROPERTIES OUTPUT_NAME Mist)
set_target_properties(MistComputingLibraryStatic PROPERTIES OUTPUT_NAME MistStatic)

set_target_properties(MistComputingLibrary PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(MistComputingLibraryStatic PROPERTIES LINKER_LANGUAGE CXX)

cmake_minimum_required(VERSION 3.16)
project(libarea_consumer LANGUAGES CXX)

find_package(libarea CONFIG REQUIRED)

enable_testing()
add_executable(arc_roundtrip_test arc_roundtrip_test.cpp)
target_link_libraries(arc_roundtrip_test PRIVATE libarea::area)
add_test(NAME arc_roundtrip COMMAND arc_roundtrip_test)

cmake_minimum_required(VERSION 3.16)
project(libarea_consumer LANGUAGES CXX)

# The installed headers need C++17 -- Clipper2 carries a std::optional member
# and reads the reference that emplace_back only returns from C++17 on. gcc and
# clang default to C++17 and never noticed; MSVC defaults to C++14 and fails to
# compile the headers. Asking for it here is a workaround: the package itself
# should demand it, via target_compile_features(area PUBLIC cxx_std_17) in
# libarea's own CMakeLists, which would reach every consumer through the
# exported target. Drop these two lines once a libarea release carries that.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libarea CONFIG REQUIRED)

enable_testing()
add_executable(arc_roundtrip_test arc_roundtrip_test.cpp)
target_link_libraries(arc_roundtrip_test PRIVATE libarea::area)
add_test(NAME arc_roundtrip COMMAND arc_roundtrip_test)

cmake_minimum_required(VERSION 3.7)
project(with_cmake)

# gen lib
add_library(withcmake_pybind_test SHARED src/test.h src/test.cc)

# bind-python
# using same path `@loader_path` during building and installation
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "@loader_path")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(pybind11)

include_directories(src)

pybind11_add_module(withcmake_pybind python/withcmake/pybind/withcmake_pybind.cc)
target_link_libraries(withcmake_pybind PRIVATE withcmake_pybind_test)

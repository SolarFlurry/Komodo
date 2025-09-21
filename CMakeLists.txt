cmake_minimum_required(VERSION 3.10)

project(Transpiler LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_executable(kmd_compiler ./src/core/main.cpp)
# add_executable(komodo_tests ./src/core/tests/tests.cpp)
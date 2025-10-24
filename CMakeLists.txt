cmake_minimum_required(VERSION 3.10)

project(Transpiler LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(SOURCES
	src/core/main.cpp
	src/core/lexer/lexer.cpp
	src/core/parser/parser.cpp
	src/core/parser/ast_utils.cpp
	src/core/helper/token.cpp
	src/core/helper/ast.cpp
	src/core/helper/error.cpp)

add_executable(kmd_compiler ${SOURCES})
# add_executable(komodo_tests ./src/core/tests/tests.cpp)
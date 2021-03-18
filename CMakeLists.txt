cmake_minimum_required(VERSION 3.0.0)
project(regex VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(tests test/test_parser.cpp test/test_compile.cpp test/test_nfa.cpp test/test_dfa.cpp src/ast.cpp src/parser.cpp src/state/state.cpp src/state/nstate.cpp src/state/dstate.cpp src/automata/nfa.cpp src/automata/dfa.cpp src/compile.cpp src/table.cpp src/utilities.cpp)
add_executable(regex src/regex.cpp src/ast.cpp src/parser.cpp src/state/state.cpp src/state/nstate.cpp src/state/dstate.cpp src/automata/nfa.cpp src/automata/dfa.cpp src/compile.cpp src/table.cpp src/utilities.cpp)

target_link_libraries(tests libgtest.a)
target_link_libraries(tests libgtest_main.a)
target_link_libraries(tests libpthread.so)
add_library(regex-lib
        language/parser.cpp
        state/nstate.cpp
        state/dstate.cpp
        automata/nfa.cpp
        automata/dfa.cpp
        utilities/compile.cpp
        language/alphabet.cpp
        cmdline.cpp)

target_include_directories(regex-lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(regex-lib PUBLIC ${PROJECT_BINARY_DIR})
install(TARGETS regex-lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/regex DESTINATION include)

add_executable(regex regex.cpp)
target_link_libraries(regex PRIVATE regex-lib)
install(TARGETS regex)

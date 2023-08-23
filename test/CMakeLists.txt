add_executable(test-regex
        test_parser.cpp
        test_compile.cpp
        test_cmdline.cpp
        test_nfa.cpp
        test_dfa.cpp
        )

if (UNIX)
    target_compile_options(test-regex PRIVATE -fsanitize=address)
    target_link_options(test-regex PRIVATE -fsanitize=address)
endif (UNIX)

target_include_directories(test-regex PRIVATE include)

target_link_libraries(test-regex PRIVATE regex-lib)
target_link_libraries(test-regex PRIVATE GTest::gtest)
target_link_libraries(test-regex PRIVATE GTest::gtest_main)
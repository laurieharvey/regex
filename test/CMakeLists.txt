add_executable(tests
        test_parser.cpp
        test_compile.cpp
        test_cmdline.cpp
        test_nfa.cpp
        test_dfa.cpp
        )

target_compile_options(tests PRIVATE -fsanitize=address)
target_link_options(tests PRIVATE -fsanitize=address)

target_include_directories(tests PRIVATE include)

target_link_libraries(tests PRIVATE regex-lib)
target_link_libraries(tests PRIVATE gtest)
target_link_libraries(tests PRIVATE gtest_main)
target_link_libraries(tests PRIVATE pthread)
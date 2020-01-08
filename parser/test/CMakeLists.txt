
set(TESTS
    "src/main.cpp"
    "src/test_errors.cpp"
)

add_executable(tests ${TESTS})
target_include_directories(tests PRIVATE oberon0c_parser gtest gtest_main)
target_link_libraries(tests PRIVATE
    oberon0c_parser
    gtest
    gtest_main
)

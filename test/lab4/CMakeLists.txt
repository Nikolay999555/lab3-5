add_executable(hamming-coder-test HammingCoderTest.cpp)
target_link_libraries(hamming-coder-test coder gtest gtest_main)
add_test(NAME hamming-coder-test COMMAND hamming-coder-test)

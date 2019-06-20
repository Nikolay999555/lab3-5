add_executable(hamming-decoder-test HammingDecoderTest.cpp)
target_link_libraries(hamming-decoder-test decoder gtest gtest_main)
add_test(NAME hamming-decoder-test COMMAND hamming-decoder-test)

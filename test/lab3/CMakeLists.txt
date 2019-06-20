add_executable(calculator-test CalculatorTest.cpp)
target_link_libraries(calculator-test calc gtest gtest_main)
add_test(NAME calculator-test COMMAND calculator-test)

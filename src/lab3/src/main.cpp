#include <src/lab3/Calculator.h>

#include <iostream>

int main()
{
  Calculator calc;

  for(;;)
  {
    std::cout << "Input expression or \"quit\": " << std::endl;
    std::string expression;
    getline(std::cin, expression); // для чтения строки целиком, а не до первого пробела
    if(expression == "quit")
      exit(0);
    calc.setExpression(expression);
    try
    {
      std::optional<double> result = calc.calculate();
      if (result)
      {
        std::cout << *result << std::endl;
      }
    }
    catch (std::logic_error &ex)
    {
      std::cout << "Error: " << ex.what() << std::endl;
    }
  }
}

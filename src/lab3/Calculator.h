#ifndef LABS_CALCULATOR_H
#define LABS_CALCULATOR_H

#include <src/lab3/util.h>

#include <optional>
#include <string>
#include <deque>
#include <variant>
#include <vector>

using Util::Operation;
using Util::Operand;

class Calculator
{
public:
  Calculator() = default;

  void setExpression(std::string expression);

  std::optional<double> calculate();

private:
  using Token = std::variant<Operation, Operand>;
  using TokenSequence = std::vector<Token>;

  std::string m_expression;
  std::vector<double> m_memory;
  std::optional<double> m_lastValue;

  void checkForUnbalancedParentheses();
  TokenSequence tokenize();
  Token extractToken(std::string &expression) const;
  bool executeMemoryWrite();
  void writeToMemory(int index = 0);
  void replaceMemoryRead(TokenSequence &sequence);
  double getFromMemory(int pos = 0);
  double process(const TokenSequence &sequence);
  const TokenSequence getPostfixNotation(const TokenSequence &sequence) const;
  double processPostfixNotation(const TokenSequence &sequence) const;
};

#endif //LABS_CALCULATOR_H

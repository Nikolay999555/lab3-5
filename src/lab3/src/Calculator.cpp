#include <src/lab3/Calculator.h>

#include <algorithm>
#include <cmath>
#include <regex>
#include <stack>
#include <string>
#include <variant>

void Calculator::setExpression(std::string expression)
{
  m_expression = std::move(expression);
}

std::optional<double> Calculator::calculate()
{
  if (executeMemoryWrite())
    return {};
  m_lastValue.reset();
  checkForUnbalancedParentheses();
  TokenSequence sequence = tokenize();
  replaceMemoryRead(sequence);
  double result = process(sequence);
  m_lastValue = result;
  return result;
}

void Calculator::checkForUnbalancedParentheses()
{
  const auto left = std::count(std::begin(m_expression), std::end(m_expression), '(');
  const auto right = std::count(std::begin(m_expression), std::end(m_expression), ')');
  if (left != right)
  {
    throw std::logic_error("Unbalanced parentheses");
  }
}

Calculator::TokenSequence Calculator::tokenize()
{
  auto expression = m_expression;
  TokenSequence result;
  while (!expression.empty())
  {
    expression.erase(0, expression.find_first_not_of(' '));
    if (!expression.empty())
    {
      Token token = extractToken(expression);
      result.push_back(token);
    }
  }
  if (result.empty())
  {
    throw std::logic_error("No token read - expression is empty");
  }
  return result;
}

Calculator::Token Calculator::extractToken(std::string &expression) const
{
  std::regex numberRegex{R"(^\d+\.?\d*)"};
  std::regex functionRegex{R"(^(MR|\^2|sqrt|mod|\+|-|\*|\/|\(|\[|\)|\]|=))"};
  std::smatch match;
  if (std::regex_search(expression, match, numberRegex))
  {
    const auto str = match[0].str();
    const auto number = std::stod(str);
    Calculator::Token token = Operand{number};
    expression.erase(0, str.size());
    return token;
  }
  else if (std::regex_search(expression, match, functionRegex))
  {
    const auto str = match[0].str();
    Operation::Type type{};
    if (str == "+")
      type = Operation::Type::PLUS;
    else if (str == "-")
      type = Operation::Type::MINUS;
    else if (str == "*")
      type = Operation::Type::MULTIPLY;
    else if (str == "/")
      type = Operation::Type::DIVIDE;
    else if (str == "mod")
      type = Operation::Type::MOD;
    else if (str == "sqrt")
      type = Operation::Type::SQRT;
    else if (str == "^2")
      type = Operation::Type::SQUARE;
    else if (str == "MR")
      type = Operation::Type::MEM_READ;
    else if (str == "(" || str == "[")
      type = Operation::Type::LEFT_PARENTHESIS;
    else if (str == ")" || str == "]")
      type = Operation::Type::RIGHT_PARENTHESIS;
    else if (str == ")")
      type = Operation::Type::RIGHT_PARENTHESIS;
    else if (str == "=")
      type = Operation::Type::EQUAL;
    Calculator::Token token = Operation{type};
    expression.erase(0, str.size());
    return token;
  }
  throw std::logic_error("Unrecognized token");
}

bool Calculator::executeMemoryWrite()
{
  std::regex memoryWriteRegex{R"(^MW(\[\d+\])?)"};
  std::smatch match;
  if (std::regex_search(m_expression, match, memoryWriteRegex))
  {
    const auto str = match[0].str();
    if (str.size() > 2) // has index
    {
      const auto indexStr = m_expression.substr(3, m_expression.size() - 4);
      const auto index = std::stoi(indexStr);
      writeToMemory(index);
    }
    else
      writeToMemory();
    return true;
  }
  return false;
}

void Calculator::writeToMemory(const int index)
{
  if (!m_lastValue)
    throw std::logic_error("No data to write to memory");

  if (m_memory.size() < (index + 1))
    m_memory.resize(index + 1);
  if (m_lastValue)
    m_memory[index] = m_lastValue.value();
}

void Calculator::replaceMemoryRead(Calculator::TokenSequence &sequence)
{
  auto iter = std::find_if(std::begin(sequence), std::end(sequence),
                           [](const Calculator::TokenSequence::value_type &token)
                           {
                             return (token.index() == 0 &&
                                     std::get<Operation>(token).getType() == Operation::Type::MEM_READ);
                           });
  if (iter != std::end(sequence))
  {
    const auto leftTokenIter = iter + 1;
    const auto valueTokenIter = iter + 2;
    const auto rightTokenIter = iter + 2 == std::end(sequence) ? std::end(sequence) : iter + 3; // MSVC iterator impl
    bool leftTokenIsParenthesis = (leftTokenIter != std::end(sequence) && leftTokenIter->index() == 0 &&
                                   std::get<Operation>(*leftTokenIter).getType() ==
                                   Operation::Type::LEFT_PARENTHESIS);
    bool valueTokenIsValue = (valueTokenIter != std::end(sequence) && valueTokenIter->index() == 1);
    bool rightTokenIsParenthesis = (rightTokenIter != std::end(sequence) && rightTokenIter->index() == 0 &&
                                       std::get<Operation>(*rightTokenIter).getType() ==
                                       Operation::Type::RIGHT_PARENTHESIS);
    if (leftTokenIsParenthesis && valueTokenIsValue && rightTokenIsParenthesis) // выражение вида MR[n]
    {
      const auto &valueToken = *valueTokenIter;
      const int arg = static_cast<int>(std::get<Operand>(valueToken).getValue());
      double value = getFromMemory(arg);
      long long offset = std::distance(std::begin(sequence), iter);
      sequence.erase(iter, iter + 4);
      sequence.insert(std::begin(sequence) + offset, Operand{value});
    }
    else // MR без аргумента
    {
      double value = getFromMemory();
      *iter = Operand{value};
    }
  }
}

double Calculator::getFromMemory(const int pos)
{
  try
  {
    return m_memory.at(pos);
  }
  catch (std::out_of_range &)
  {
    throw std::logic_error("No data in memory to read");
  }
}

double Calculator::process(const Calculator::TokenSequence &sequence)
{
  const Calculator::TokenSequence rpn = getPostfixNotation(sequence);
  const double result = processPostfixNotation(rpn);
  return result;
}

const Calculator::TokenSequence Calculator::getPostfixNotation(const Calculator::TokenSequence &sequence) const
{
  auto isInfix = [](const Calculator::Token &token)
  {
    return Util::isInfix(std::get<Operation>(token).getType());
  };
  auto isPrefix = [](const Calculator::Token &token)
  {
    return Util::isPrefix(std::get<Operation>(token).getType());
  };
  auto isPostfix = [](const Calculator::Token &token)
  {
    return Util::isPostfix(std::get<Operation>(token).getType());
  };
  auto priority = [](const Calculator::Token &token)
  {
    return Util::priority(std::get<Operation>(token).getType());
  };

  std::stack<Calculator::Token> stack;
  Calculator::TokenSequence result;
  for (const auto &token : sequence)
  {
    if (token.index() == 1 || isPostfix(token))
      result.push_back(token);
    else if (isPrefix(token) || std::get<Operation>(token).getType() == Operation::Type::LEFT_PARENTHESIS)
      stack.push(token);
    else if (std::get<Operation>(token).getType() == Operation::Type::RIGHT_PARENTHESIS)
    {
      for (;;)
      {
        auto top = stack.top();
        stack.pop();
        if (top.index() == 0 && std::get<Operation>(top).getType() == Operation::Type::LEFT_PARENTHESIS)
          break;
        result.push_back(top);
      }
    }
    else if (isInfix(std::get<Operation>(token)))
    {
      while (!stack.empty() && stack.top().index() == 0 &&
             (isPrefix(stack.top()) || priority(stack.top()) >= priority(token)))
      {
        result.push_back(stack.top());
        stack.pop();
      }
      stack.push(token);
    }
  }
  while (!stack.empty())
  {
    result.push_back(stack.top());
    stack.pop();
  }
  return result;
}

double Calculator::processPostfixNotation(const Calculator::TokenSequence &sequence) const
{
  std::stack<Token> stack;
  auto applyOp = [&stack](auto op) -> double
  {
    if (stack.size() < 2)
      throw std::logic_error("Not enough operands to perform operation");

    try
    {
      auto rhs = std::get<Operand>(stack.top()).getValue();
      stack.pop();
      auto lhs = std::get<Operand>(stack.top()).getValue();
      stack.pop();
      return op(lhs, rhs);
    }
    catch (std::bad_variant_access &)
    {
      throw std::logic_error("Got operation instead of number; expression is incorrect");
    }
  };

  for (const auto &token : sequence)
  {
    if (token.index() == 1)
      stack.push(token);
    else
    {
      double value;
      switch (std::get<Operation>(token).getType())
      {
        case Operation::Type::SQUARE:
          value = std::get<Operand>(stack.top()).getValue();
          value *= value;
          stack.pop();
          break;
        case Operation::Type::SQRT:
          value = std::get<Operand>(stack.top()).getValue();
          value = sqrt(value);
          stack.pop();
          break;
        case Operation::Type::PLUS:
          value = applyOp([](double lhs, double rhs)
                          {
                            return lhs + rhs;
                          });
          break;
        case Operation::Type::MINUS:
          value = applyOp([](double lhs, double rhs)
                          {
                            return lhs - rhs;
                          });
          break;
        case Operation::Type::MULTIPLY:
          value = applyOp([](double lhs, double rhs)
                          {
                            return lhs * rhs;
                          });
          break;
        case Operation::Type::DIVIDE:
          value = applyOp([](double lhs, double rhs)
                          {
                            if (rhs == 0.0)
                              throw std::logic_error("Division by zero");
                            return lhs / rhs;
                          });
          break;
        case Operation::Type::MOD:
          value = applyOp([](double lhs, double rhs)
                          {
                            if (rhs == 0.0)
                              throw std::logic_error("Division by zero");
                            return static_cast<int>(lhs) % static_cast<int>(rhs);
                          });
          break;
        default:
          throw std::logic_error("Token found where it should not be");
      }
      stack.push(Operand{value});
    }
  }
  return std::get<Operand>(stack.top()).getValue();
}

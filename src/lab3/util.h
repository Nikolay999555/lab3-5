#ifndef LABS_UTIL_H
#define LABS_UTIL_H

namespace Util
{
  class Operation
  {
  public:
    enum class Type
    {
      PLUS,
      MINUS,
      MULTIPLY,
      DIVIDE,
      MOD,
      SQRT,
      SQUARE,
      MEM_READ,
      LEFT_PARENTHESIS,
      RIGHT_PARENTHESIS,
      EQUAL,
      NOOP
    };

    Operation() : m_type(Type::NOOP)
    {
    }

    explicit Operation(const Type type) : m_type(type)
    {
    }

    Type getType() const
    {
      return m_type;
    }

  private:
    Type m_type;
  };

  bool isPrefix(Operation::Type type);
  bool isPostfix(Operation::Type type);
  bool isInfix(Operation::Type type);
  int priority(Operation::Type type);

  class Operand
  {
  public:
    explicit Operand(double value) : m_value(value)
    {
    }

    double getValue() const
    {
      return m_value;
    }

  private:
    double m_value;
  };
}

#endif //LABS_UTIL_H

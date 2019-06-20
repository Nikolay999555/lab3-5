#include <src/lab3/util.h>

namespace Util
{
  bool isPrefix(const Operation::Type type)
  {
    return type == Operation::Type::SQRT;
  }

  bool isPostfix(const Operation::Type type)
  {
    return type == Operation::Type::SQUARE;
  }

  bool isInfix(const Operation::Type type)
  {
    return (type == Operation::Type::PLUS ||
            type == Operation::Type::MINUS ||
            type == Operation::Type::MULTIPLY ||
            type == Operation::Type::DIVIDE ||
            type == Operation::Type::MOD
    );
  }

  int priority(const Operation::Type type)
  {
    switch (type)
    {
      case Operation::Type::SQUARE:
        [[fallthrough]];
      case Operation::Type::SQRT:
        return 2;
      case Operation::Type::MULTIPLY:
        [[fallthrough]];
      case Operation::Type::DIVIDE:
        [[fallthrough]];
      case Operation::Type::MOD:
        return 1;
      case Operation::Type::PLUS:
        [[fallthrough]];
      case Operation::Type::MINUS:
        return 0;
      default:
        return -1;
    }
  }
}

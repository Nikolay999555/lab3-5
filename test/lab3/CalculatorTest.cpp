#include <gtest/gtest.h>

#include <src/lab3/Calculator.h>

class CalculatorTest : public ::testing::Test
{
public:
  void SetUp() override
  {
    m_instance = std::make_unique<decltype(m_instance)::element_type>();
  }

protected:
  std::unique_ptr<Calculator> m_instance;
};

TEST_F(CalculatorTest, calculateOk)
{
  m_instance->setExpression("25^2 -sqrt(225) + 5- (6/3 + 0.5) = ");
  std::optional<double> result;
  ASSERT_NO_THROW(result = m_instance->calculate());
  ASSERT_TRUE(result);
  ASSERT_DOUBLE_EQ(612.5, result.value());

  m_instance->setExpression("MW");
  ASSERT_NO_THROW(result = m_instance->calculate());
  ASSERT_FALSE(result);

  result = {};
  m_instance->setExpression("12.5 + MR = ");
  ASSERT_NO_THROW(result = m_instance->calculate());
  ASSERT_TRUE(result);
  ASSERT_DOUBLE_EQ(625, result.value());
}

TEST_F(CalculatorTest, testMemoryRead)
{
  m_instance->setExpression("25 =");
  ASSERT_NO_THROW(m_instance->calculate());
  m_instance->setExpression("MW");
  ASSERT_NO_THROW(m_instance->calculate());

  m_instance->setExpression("MR + 5 = ");
  std::optional<double> result;
  ASSERT_NO_THROW(result = m_instance->calculate());
  ASSERT_TRUE(result);
  ASSERT_DOUBLE_EQ(30, result.value());
}

TEST_F(CalculatorTest, calculateThrowsWhenIncorrectExpression)
{
  m_instance->setExpression("25 + -3");
  std::optional<double> result;
  ASSERT_THROW(result = m_instance->calculate(), std::logic_error);
  ASSERT_FALSE(result);


  m_instance->setExpression("25 + 3 * -1");
  ASSERT_THROW(result = m_instance->calculate(), std::logic_error);
  ASSERT_FALSE(result);
}

TEST_F(CalculatorTest, testInvert)
{
  m_instance->setExpression("1/3 = ");
  std::optional<double> result;
  ASSERT_NO_THROW(result = m_instance->calculate());
  ASSERT_TRUE(result);
  ASSERT_DOUBLE_EQ(1 / 3.0, result.value());

  m_instance->setExpression("1/4 + 0.75 = ");
  result.reset();
  ASSERT_NO_THROW(result = m_instance->calculate());
  ASSERT_TRUE(result);
  ASSERT_DOUBLE_EQ(1, result.value());
}

TEST_F(CalculatorTest, calculateThrowsWhenDivideByZero)
{
  m_instance->setExpression("25 / 0 =");
  ASSERT_THROW(m_instance->calculate(), std::logic_error);
}

TEST_F(CalculatorTest, calculateThrowsWhenUnbalancedParentheses)
{
  m_instance->setExpression("25 + (5 - (3 * 2) =");
  ASSERT_THROW(m_instance->calculate(), std::logic_error);
}

TEST_F(CalculatorTest, calculateThrowsWhenUnrecognizedSymbol)
{
  m_instance->setExpression("25 * 2 + n = ");
  ASSERT_THROW(m_instance->calculate(), std::logic_error);
}

TEST_F(CalculatorTest, calculateThrowsWhenNoDataInMemory)
{
  m_instance->setExpression("25 + MR = ");
  EXPECT_THROW(m_instance->calculate(), std::logic_error);
}

TEST_F(CalculatorTest, calculateThrowsWhenNoDataToWrite)
{
  m_instance->setExpression("MW");
  EXPECT_THROW(m_instance->calculate(), std::logic_error);
}

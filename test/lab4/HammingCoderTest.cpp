#include <src/lab4/Hamming74Coder.h>

#include <gtest/gtest.h>

#include <cstdint>
#include <numeric>
#include <vector>

class HammingCoderTest : public ::testing::Test
{
public:
  void SetUp() override
  {
    m_instance = new Hamming74Coder;
    assert(m_instance);
  }

  void TearDown() override
  {
    delete m_instance;
  }

protected:
  ICoder *m_instance = nullptr;
};

TEST_F(HammingCoderTest, testEmpty)
{
  std::vector<uint8_t> data;
  const std::vector<uint8_t> encoded = m_instance->encode(data);
  ASSERT_TRUE(encoded.empty());
}

TEST_F(HammingCoderTest, testEncode)
{
  std::vector<uint8_t> data(16);
  std::iota(std::begin(data), std::end(data), 0); // заполнение от 0 до 15
  const std::vector<uint8_t> encoded = m_instance->encode(data);
  const std::vector<uint8_t> expected{0b0000'000, 0b0001'011, 0b0010'110, 0b0011'101,
                                      0b0100'111, 0b0101'100, 0b0110'001, 0b0111'010,
                                      0b1000'101, 0b1001'110, 0b1010'011, 0b1011'000,
                                      0b1100'010, 0b1101'001, 0b1110'100, 0b1111'111};
  ASSERT_EQ(expected.size(), encoded.size());
  for (size_t i = 0; i < expected.size(); ++i)
  {
    EXPECT_EQ(expected[i], encoded[i]) << "i = " << i;
  }
}

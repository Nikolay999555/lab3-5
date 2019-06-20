#include <src/lab5/Hamming74Decoder.h>

#include <gtest/gtest.h>

#include <numeric>

class HammingDecoderTest : public ::testing::Test
{
public:
  void SetUp() override
  {
    m_instance = new Hamming74Decoder;
    assert(m_instance);
  }

  void TearDown() override
  {
    delete m_instance;
  }

protected:
  IDecoder *m_instance = nullptr;
};

TEST_F(HammingDecoderTest, testEmpty)
{
  const std::vector<uint8_t> data;
  const auto decoded = m_instance->decode(data);
  ASSERT_TRUE(decoded.empty());
}

TEST_F(HammingDecoderTest, testDecode)
{
  const std::vector<uint8_t> data{0b0000'000, 0b0001'011, 0b0010'110, 0b0011'101,
                                  0b0100'111, 0b0101'100, 0b0110'001, 0b0111'010,
                                  0b1000'101, 0b1001'110, 0b1010'011, 0b1011'000,
                                  0b1100'010, 0b1101'001, 0b1110'100, 0b1111'111};
  std::vector<uint8_t> expected(16);
  std::iota(std::begin(expected), std::end(expected), 0);
  const std::vector<uint8_t> decoded = m_instance->decode(data);
  ASSERT_EQ(expected.size(), decoded.size());
  for (size_t i = 0; i < expected.size(); ++i)
  {
    EXPECT_EQ(expected[i], decoded[i]) << "i = " << i;
  }
}

TEST_F(HammingDecoderTest, testFixError)
{
  const std::vector<uint8_t> data{0b1100'111, 0b0000'111, 0b0110'111, 0b0101'111, 0b0100'011, 0b0100'101, 0b0100'110};
  const auto decoded = m_instance->decode(data);
  const auto count = std::count(std::begin(decoded), std::end(decoded), 0b0100);
  ASSERT_EQ(count, 7);
}

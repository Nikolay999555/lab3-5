#include <src/lab4/Hamming74Coder.h>

#include <bitset>

std::vector<uint8_t> Hamming74Coder::encode(const std::vector<uint8_t> &data)
{
  std::vector<uint8_t> result = data;
  for (auto &item : result)
  {
    const std::bitset<4> bs{item};

    const uint8_t r0 = bs[3] ^bs[2] ^bs[1];
    const uint8_t r1 = bs[2] ^bs[1] ^bs[0];
    const uint8_t r2 = bs[3] ^bs[2] ^bs[0];
    uint8_t added = (r0 << 2u) | (r1 << 1u) | r2;
    item = item << 3u | added;
  }

  return result;
}

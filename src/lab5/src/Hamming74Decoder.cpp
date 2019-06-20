#include <src/lab5/Hamming74Decoder.h>

#include <bitset>

std::vector<uint8_t> Hamming74Decoder::decode(const std::vector<uint8_t>& data)
{
  auto result = data;
  for (auto& item : result)
  {
    const std::bitset<7> bs{item};
    uint8_t i1 = bs[6];
    uint8_t i2 = bs[5];
    uint8_t i3 = bs[4];
    uint8_t i4 = bs[3];

    // вычисление синдрома
    const auto s1 = bs[2] ^bs[6] ^bs[5] ^bs[4];
    const auto s2 = bs[1] ^bs[5] ^bs[4] ^bs[3];
    const auto s3 = bs[0] ^bs[6] ^bs[5] ^bs[3];
    const auto syndrome = s1 << 2 | s2 << 1 | s3;

    switch (syndrome)
    {
      case 0b011:
        i4 = !i4;
        break;
      case 0b101:
        i1 = !i1;
        break;
      case 0b110:
        i3 = !i3;
        break;
      case 0b111:
        i2 = !i2;
        break;
      case 0b000:
        [[fallthrough]];
      case 0b001:
        [[fallthrough]];
      case 0b010:
        [[fallthrough]];
      case 0b100:
      default:
        break;
    }

    item = i1 << 3 | i2 << 2 | i3 << 1 | i4;
  }
  return result;
}

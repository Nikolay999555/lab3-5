#ifndef LABS_IDECODER_H
#define LABS_IDECODER_H

#include <cstdint>
#include <vector>

class IDecoder
{
public:
  virtual ~IDecoder() = default;

  virtual std::vector<uint8_t> decode(const std::vector<uint8_t> &data) = 0;
};

#endif //LABS_IDECODER_H

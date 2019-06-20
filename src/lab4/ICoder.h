#ifndef LABS_ICODER_H
#define LABS_ICODER_H

#include <cstdint>
#include <vector>

class ICoder
{
public:
  virtual ~ICoder() = default;

  virtual std::vector<uint8_t> encode(const std::vector<uint8_t> &data) = 0;
};

#endif //LABS_ICODER_H

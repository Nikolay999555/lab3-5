#ifndef LABS_HAMMING74DECODER_H
#define LABS_HAMMING74DECODER_H

#include <src/lab5/IDecoder.h>

class Hamming74Decoder : public IDecoder
{
public:
  /**
   * Декодирует последовательность элементов.
   * @param data Данные для декодирования
   * @note Игнорирует старший бит каждого байта, т.е. каждое число нужно класть отдельным элементом.
   * @return Вектор байт, состоящий из декодированных элементов.
   */
  std::vector<uint8_t> decode(const std::vector<uint8_t> &data) override;
};


#endif //LABS_HAMMING74DECODER_H

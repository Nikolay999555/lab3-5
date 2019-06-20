#ifndef LABS_HAMMING74CODER_H
#define LABS_HAMMING74CODER_H

#include <src/lab4/ICoder.h>

class Hamming74Coder : public ICoder
{
public:
  /**
   * Вычисляет код Хэмминга для заданной последовательности байт.
   * @param data Данные для кодирования
   * @note Обрабатывает только младшие 4 бита каждого байта, т.е. каждое число нужно класть отдельным элементом.
   * @return Вектор байт, состоящий из кодов Хэмминга для каждого элемента исходного вектора.
   */
  std::vector<uint8_t> encode(const std::vector<uint8_t> &data) override;
};


#endif //LABS_HAMMING74CODER_H

#ifndef LCD_HPP
#define LCD_HPP

#include <LCD-I2C.h>
#include "screen.hpp"

class LcdRenderer : public LCD_I2C {
public:
  LcdRenderer(uint8_t addr, uint8_t cols, uint8_t rows)
    : LCD_I2C(addr, cols, rows)
  {
  }

  void render(const Lcd1602Screen *screen)
  {
    if (screen == nullptr) {
      return;
    }

    auto  cols = screen->cols();
    auto  rows = screen->rows();
    auto& buf  = screen->buffer();
    for (auto row = 0; row < rows; ++row) {
      setCursor(0, row);
      for (auto col = 0; col < cols; ++col) {
        write(buf[row][col]);
      }
    }

    // TODO: Check if no button is pressed and turn off backlight for save power
  }

};

#endif
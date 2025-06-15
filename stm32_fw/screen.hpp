#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "Print.h"

template<uint16_t Columns, uint16_t Rows>
class LcdScreen {
public:
  class Painter : public Print {
    friend class LcdScreen;
  public:
    Painter() 
      : col_(0), row_(0) { clear(); }

    void clear() noexcept
    {
      for (auto row = 0; row < Rows; ++row) {
        for (auto col = 0; col < Columns; ++col) {
          buf_[row][col] = ' ';
        }
      }
    }

    void setCursor(uint8_t col, uint8_t row) noexcept
    {
      row_ = row % Rows;
      col_ = col % Columns;
    }

    virtual size_t write(uint8_t c) override
    {
      buf_[row_][col_] = c;
      col_ = (col_ + 1) % Columns;
      return 1;
    }

    template<typename... Args>
    size_t printAt(uint8_t col, uint8_t row, Args&&... args) {
      setCursor(col, row);
      return print(std::forward<Args>(args)...);
    }

  private:
    uint8_t buf_[Rows][Columns];
    uint8_t col_, row_;
  };

  virtual ~LcdScreen() = default;
  virtual void onEnter(void) = 0;
  virtual LcdScreen<Columns, Rows>* update(void) = 0;
  uint16_t cols() const noexcept { return Columns; }
  uint16_t rows() const noexcept { return Rows; }

  Painter& painter() noexcept { return painter_; }
  const Painter& painter() const noexcept { return painter_; }

  // non-const + const overloads
  uint8_t (&buffer())[Rows][Columns] { return painter_.buf_; }
  const uint8_t (&buffer() const)[Rows][Columns] { return painter_.buf_; }

protected:
  Painter painter_;
};

using Lcd1602Screen = LcdScreen<LCD_COLS, LCD_ROWS>;

#endif
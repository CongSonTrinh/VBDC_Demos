#ifndef MENU_HPP
#define MENU_HPP

#include "screen.hpp"

class MenuScreen : public Lcd1602Screen {
public:
  struct Item {
    const char *label;
    Lcd1602Screen *next;
  };

  MenuScreen(const Item *items, uint8_t size, Lcd1602Screen *parent)
    : items_(items), totalItem_(size), parent_(parent)
  {
    init();
  }

protected:
  void init() { }

  void move_up()
  {
    if (currentItem_ == 0) return;
    currentItem_--;

    if (currentCursor_ == 0) return;
    currentCursor_--;
  }

  void move_down()
  {
    if ((totalItem_ - 1) == currentItem_) return;
    currentItem_++;

    if ((rows() - 1) == currentCursor_) return;
    currentCursor_++;
  }

  void drawMenu()
  {
    uint8_t startItemIndex = totalItem_ > rows() ? (currentItem_ - currentCursor_) : 0;

    painter_.clear();
    for (uint8_t row = 0; row < rows(); ++row) {
      uint8_t itemIndex = startItemIndex + row;
      painter_.setCursor(0, row);
      if (itemIndex == currentItem_) {
        painter_.print('>');
      } else {
        painter_.print(' ');
      }
      painter_.print(items_[itemIndex].label);       
    }
  }

private:
  const Item    *items_;
  uint8_t        totalItem_;
  uint8_t        currentItem_;
  uint8_t        currentCursor_;
  Lcd1602Screen *parent_;
};

using MenuItem = MenuScreen::Item;

#endif

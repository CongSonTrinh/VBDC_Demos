#ifndef SCREEN_APP_HPP
#define SCREEN_APP_HPP

#include "menu.hpp"
#include "button.hpp"

class RootMenuScreen : public MenuScreen {
public:
  RootMenuScreen(const Item *items, uint8_t size, Lcd1602Screen *parent)
  : MenuScreen(items, size, parent)
  {
  }

  void onEnter(void)
  {
    drawMenu();
  }

  Lcd1602Screen* update(void) override
  {
    if (buttonIsPressed(BUTTON_DOWN)) move_down();
    else if (buttonIsPressed(BUTTON_UP)) move_up();

    drawMenu();
    return this;
  }
};

#endif
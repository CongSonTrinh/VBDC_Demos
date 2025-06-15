#ifndef SCREEN_ENGINE_HPP
#define SCREEN_ENGINE_HPP

#include "screen.hpp"

class ScreenEngine {
public:
  explicit ScreenEngine() : current_(nullptr) {}

  void begin(Lcd1602Screen* startScreen)
  {
    if (startScreen) {
      current_ = startScreen;
      current_->onEnter();
    }
  }

  Lcd1602Screen* loop()
  {
    Lcd1602Screen* next = current_->update();
    if (next && next != current_) {
      current_ = next;
      current_->onEnter();
    }

    return current_;
  }

private:
    Lcd1602Screen* current_;
};

#endif
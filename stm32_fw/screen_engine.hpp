#ifndef SCREEN_ENGINE_HPP
#define SCREEN_ENGINE_HPP

#include "screen.hpp"

class ScreenEngine {
public:
  explicit ScreenEngine() : current_(nullptr) {}

  void begin(Lcd1602Screen* start)
  {
    if (!start) return;
    current_ = start;
    current_->onEnter();
  }

  Lcd1602Screen* loop()
  {
    Lcd1602Screen* next = current_->update();
    if (next != current_) {
      current_ = next;
      if (current_) current_->onEnter();
    }

    return current_;
  }

private:
    Lcd1602Screen* current_;
};

#endif
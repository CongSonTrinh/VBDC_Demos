#ifndef SCREEN_APP_HPP
#define SCREEN_APP_HPP

#include "menu.hpp"
#include "button.hpp"
#include "led.hpp"
#include "rtc.hpp"

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
    if (buttonIsPressed(BUTTON_DOWN)) {
      move_down();
    } else if (buttonIsPressed(BUTTON_UP)) {
      move_up();
    } else if (buttonIsPressed(BUTTON_SET)) {
      return nextScreen();
    }

    drawMenu();
    return this;
  }
};

class RtcTimeScreen : public Lcd1602Screen {
public:
  RtcTimeScreen(Lcd1602Screen *parent)
  : parent_(parent)
  {
  }

  void onEnter(void) {
    rtcData_ = getRTCData();
    printTime();
  }

  Lcd1602Screen* update(void) {
    printTime();

    if (buttonIsPressed(BUTTON_MODE)) {
      return parent_;
    }

    return this;
  }

private:
  RtcData *rtcData_;

  // Example format
  // Wed 25 Jul 2025
  // 14:25 Tp: 30.40
  void printTime() {
    String dateLine, hourLine;

    auto& rtcData = *rtcData_;

    dateLine += dayOfWeekString(rtcData.dayOfWeek);
    dateLine += ' ';
    if (rtcData.date < 10) {
      dateLine += (String(0) + rtcData.date);
    } else {
      dateLine += rtcData.date;
    }
    dateLine += ' ';
    dateLine += monthOfYearString(rtcData.month);
    dateLine += ' ';
    dateLine += (2000 + rtcData.year);

    if (rtcData.hour < 10) {
      hourLine += (String(0) + rtcData.hour);
    } else {
      hourLine += rtcData.hour;
    }
    hourLine += ':';
    if (rtcData.minute < 10) {
      hourLine += (String(0) + rtcData.minute);
    } else {
      hourLine += rtcData.minute;
    }
    hourLine += ':';
    if (rtcData.second < 10) {
      hourLine += (String(0) + rtcData.second);
    } else {
      hourLine += rtcData.second;
    }
    hourLine += " T: ";
    hourLine += String(rtcData.temperature, 1);

    painter_.setCursor(0, 0);
    painter_.print(dateLine);
    painter_.setCursor(0, 1);
    painter_.print(hourLine);
  }

  Lcd1602Screen *parent_;
};

class LedControlScreen : public MenuScreen {
public:
  LedControlScreen(const Item *items, uint8_t size, Lcd1602Screen *parent)
    : MenuScreen(items, size, parent)
  {
  }

  void onEnter(void)
  {
    setCursor(0);
    setItemIndex(0);
    LedControlScreen::drawMenu();
  }

  Lcd1602Screen* update(void) override
  {
    if (buttonIsPressed(BUTTON_DOWN)) {
      move_down();
    } else if (buttonIsPressed(BUTTON_UP)) {
      move_up();
    } else if (buttonIsPressed(BUTTON_SET)) {
      ledToggle(currentItem_);
    } else if (buttonIsPressed(BUTTON_MODE)) {
      return parent();
    }

    LedControlScreen::drawMenu();

    return this;
  }

protected:
  void drawMenu(void)
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
      painter_.print(itemIndex + 1, DEC);
      painter_.print('.');
      painter_.print(items_[itemIndex].label);
      if (ledState(itemIndex) == LED_ON) {
        painter_.print("ON");
      } else {
        painter_.print("OFF");
      }
    }
  }
};

#endif
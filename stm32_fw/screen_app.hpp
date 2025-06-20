#include <sys/_stdint.h>
#ifndef SCREEN_APP_HPP
#define SCREEN_APP_HPP

#include "menu.hpp"
#include "button.hpp"
#include "led.hpp"
#include "rtc.hpp"
#include "aht10.hpp"

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

    painter_.printAt(0, 0, dateLine);
    painter_.printAt(0, 1, hourLine);
  }

  Lcd1602Screen *parent_;
};

class SensorAht10Screen : public Lcd1602Screen {
public:
  SensorAht10Screen(Lcd1602Screen *parent)
  : parent_(parent)
  {
  }

  void onEnter(void) {
    printSensor();
  }

  Lcd1602Screen* update(void) {
    static uint8_t timer_ticks = 0;
    if (timer_ticks == 0) {
      printSensor();
    }
    timer_ticks = (timer_ticks + 1) % 20;

    if (buttonIsPressed(BUTTON_MODE)) {
      return parent_;
    }

    return this;
  }

private:
  // Example format
  // 0123456789012345
  // T: xx.xx deg C
  // H: xx.xx %rH
  void printSensor() {
    String tLine, hLine;

    tLine = "T: " + String(tempAHT10(), 2) + " deg C";
    hLine = "H: " + String(humidityAHT10(), 2) + " %rH";

    painter_.clear();
    painter_.printAt(0, 0, tLine);
    painter_.printAt(0, 1, hLine);
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

class RtcSettingScreen : public Lcd1602Screen {
public:
  RtcSettingScreen(Lcd1602Screen *parent)
  : parent_(parent)
  {
  }

  enum TimeSettings {
    SETTING_DOW = 0,
    SETTING_YEAR,
    SETTING_MONTH,
    SETTING_DATE,
    SETTING_HOUR,
    SETTING_MIN,
    SETTING_NUM
  };

  void onEnter(void)
  {
    setting_ = SETTING_DOW;
    configs_ = *getRTCData();
    painter_.clear();
    printTime();
  }

  Lcd1602Screen* update(void)
  {
    if (buttonIsPressed(BUTTON_SET)) {
      setting_ = (setting_ + 1) % SETTING_NUM;
    }
    if (buttonIsPressed(BUTTON_MODE)) {
      setRTC(configs_);
      return parent_;
    }
    if (buttonIsPressed_1(BUTTON_DOWN)) {
      setting(-1);
    }
    if (buttonIsPressed_1(BUTTON_UP)) {
      setting(+1);
    }

    printTime();
    blink();

    return this;
  }

private:
  RtcData configs_;

  // Example format
  // Wed 25 Jul 2025
  // 14:25 Tp: 30.40
  void printTime()
  {
    String dateLine, hourLine;

    auto& rtcData = configs_;

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

    painter_.printAt(0, 0, dateLine);
    painter_.printAt(0, 1, hourLine);
  }

  void blink()
  {
    //  0123456789012345
    //0 Fri 13 Jan 2000
    //1 22:36
    static uint8_t time_ticks = 0;

    time_ticks = (time_ticks + 1) % 20;

    if (time_ticks < 10) {
      return;
    }

    switch (setting_) {
      case SETTING_DOW:
        painter_.printAt(0, 0, "   ");
        break;
      case SETTING_YEAR:
        painter_.printAt(11, 0, "    ");
        break;
      case SETTING_MONTH:
        painter_.printAt(7, 0, "   ");
        break;
      case SETTING_DATE:
        painter_.printAt(4, 0, "  ");
        break;
      case SETTING_HOUR:
        painter_.printAt(0, 1, "  ");
        break;
      case SETTING_MIN:
        painter_.printAt(3, 1, "  ");
        break;
      default:
        break;
    }
  }

  void setting(int step)
  {
    switch(step) {
      case -1:
      case +1:
        break;

      default:
        return;
    }

    switch (setting_) {
      case SETTING_DOW:
        configs_.dayOfWeek = wrap_cyclic(configs_.dayOfWeek, step, 1, 7);
        break;
      case SETTING_YEAR:
        configs_.year = wrap_cyclic(configs_.year, step, 0, 99);
        break;
      case SETTING_MONTH:
        configs_.month = wrap_cyclic(configs_.month, step, 1, 12);
        break;
      case SETTING_DATE:
        configs_.date = wrap_cyclic(configs_.date, step, 1, endOfMonth(configs_.month, configs_.year));
        break;
      case SETTING_HOUR:
        configs_.hour = wrap_cyclic(configs_.hour, step, 0, 23);
        break;
      case SETTING_MIN:
        configs_.minute = wrap_cyclic(configs_.minute, step, 0, 59);
        break;

      default:
        break;
    }
  }

  int wrap_cyclic(uint8_t value, int step, int min, int max) {
      int range = max - min + 1;
      int offset = (value - min + step) % range;
      if (offset < 0) offset += range;
      return min + offset;
  }

  Lcd1602Screen *parent_;
  uint8_t setting_;
};

#endif
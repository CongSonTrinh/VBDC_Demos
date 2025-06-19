#include "Arduino.h"
#include "task.hpp"
#include "led.hpp"
#include "button.hpp"
#include "lcd.hpp"
#include "Wire.h"
#include "screen_app.hpp"
#include "screen_engine.hpp"
#include "aht10.hpp"

#define SIZE_OF_STATIC_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))

ScreenEngine screenEngine;
LcdRenderer lcdDisp(LCD_ADDR, LCD_COLS, LCD_ROWS);

extern MenuItem settingItems[];
extern MenuItem rootItems[];
extern RootMenuScreen rootMenuScreen;
extern LedControlScreen settingScreen;
extern RtcTimeScreen rtcTimeScreen;
extern SensorAht10Screen sensorAht10Screen;

MenuItem rootItems[] =
{
  { "RTC Time"  , &rtcTimeScreen     },
  { "Temp/Humid", &sensorAht10Screen },
  { "Led On/Off", &settingScreen     },
  { "Time Set"  , nullptr            },
};
MenuItem settingItems[] =
{
  { "Led 1:", nullptr },
  { "Led 2:", nullptr },
  { "Led 3:", nullptr },
  { "Led 4:", nullptr },
};

RootMenuScreen rootMenuScreen(rootItems, SIZE_OF_STATIC_ARRAY(rootItems), nullptr);
LedControlScreen settingScreen(settingItems, SIZE_OF_STATIC_ARRAY(settingItems), &rootMenuScreen);
RtcTimeScreen rtcTimeScreen(&rootMenuScreen);
SensorAht10Screen sensorAht10Screen(&rootMenuScreen);

void initLcd(TwoWire *i2c)
{
  Wire.begin();
  lcdDisp.begin(i2c);
  lcdDisp.display();
  lcdDisp.home();
  lcdDisp.backlight();
  screenEngine.begin(&rootMenuScreen);
}

void systemInit(void)
{
  buttonInit();
  ledInit();
  // initUart();
  initAHT10(&Wire);
  initRtc(&Wire);
  initLcd(&Wire);
}

void taskHandleScreen(void)
{
  auto screen = screenEngine.loop();
  lcdDisp.render(screen);
}

void taskUpdateRtcTime(void)
{
  static uint8_t time_ticks = 0;
  time_ticks = (time_ticks + 1) % 10;
  if (time_ticks == 0) {
    rtcUpdate();
  }
}

void taskUpdateLed(void)
{
  ledUpdate();
}

void taskUpdateTempAndHumid(void)
{
  updateAHT10();
}

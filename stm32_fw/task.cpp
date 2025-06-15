#include "task.hpp"
#include "led.hpp"
#include "button.hpp"
#include "lcd.hpp"
#include "Wire.h"
#include "screen_app.hpp"
#include "screen_engine.hpp"
#include "Arduino.h"

static LcdRenderer lcdDisp(LCD_ADDR, LCD_COLS, LCD_ROWS);
static MenuItem rootItems[] =
{
  {"1.Show Time", nullptr },
  {"2.Sensors", nullptr },
  {"3.Settings", nullptr },
  {"4.Exit", nullptr },
};
static RootMenuScreen rootMenuScreen(rootItems, sizeof(rootItems)/sizeof(rootItems[0]), nullptr);
static ScreenEngine screenEngine;

void initLcd(void)
{
  Wire.begin();
  lcdDisp.begin(&Wire);
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
  // initSensor();
  // initRtc();
  initLcd();
}

void taskHandleScreen(void)
{
  auto screen = screenEngine.loop();
  lcdDisp.render(screen);
}

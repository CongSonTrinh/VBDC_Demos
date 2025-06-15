#include "button.hpp"

struct ButtonConfig {
  u16 _pin;
  u32 _cnt;
};

#define BTN_PIN(i, suffix) i ## suffix
#define BTN_CFG(i, s)  [i] = { ._pin = BTN_PIN(i, _PIN), ._cnt = s }

static struct ButtonConfig configs[BUTTON_NUM] =
{
  BTN_CFG(BUTTON_SET , 0),
  BTN_CFG(BUTTON_MODE, 0),
  BTN_CFG(BUTTON_UP  , 0),
  BTN_CFG(BUTTON_DOWN, 0),
};

void buttonInit(void)
{
  for (u8 i = 0;i < 4; i++) {
    pinMode(configs[i]._pin, INPUT);
  }
}

void buttonScan(void)
{
  for (u8 button = 0; button < 4; button++) {
    if (digitalRead(configs[button]._pin) == BUTTON_PRESSED) {
      configs[button]._cnt++;  
    } else {
      configs[button]._cnt = 0;
    }
  }
}

bool buttonIsPressed(u16 button, u32 start_cmp)
{
  if (configs[button]._cnt == start_cmp) {
    return 1;
  } else {
    return 0;
  }
}

bool buttonIsHold(u16 button, u32 start_cmp)
{
  if (configs[button]._cnt > start_cmp) {
    return 1;
  } else {
    return 0;
  }
}

#include "led.hpp"

struct LedConfig {
  u16 _pin;
  u8  _state;
};

#define LED_PIN(i, suffix)  i ## suffix
#define LED_CFG(i, s)  [i]={._pin = LED_PIN(i, _PIN), ._state = s}

static struct LedConfig configs[LED_NUM] = 
{
  LED_CFG(LED_1, LED_OFF),
  LED_CFG(LED_2, LED_OFF),
  LED_CFG(LED_3, LED_OFF),
  LED_CFG(LED_4, LED_OFF),
};

void ledInit(void)
{
  for (u8 led = 0;led < LED_NUM; ++led)
  {
    pinMode(configs[led]._pin, OUTPUT);
    digitalWrite(configs[led]._pin, configs[led]._state);
  }
}

void ledSet(u16 led, u8 state)
{
  configs[led]._state = state;
}

void ledToggle(u16 led)
{
  if (configs[led]._state == LED_ON) {
    configs[led]._state = LED_OFF;
  } else {
    configs[led]._state = LED_ON;
  }
}

void ledUpdate(void)
{
  for (u8 led = 0; led < LED_NUM; ++led)
  {
    digitalWrite(configs[led]._pin, configs[led]._state);
  }
}

#ifndef LED_HPP
#define LED_HPP

#include "sys_config.h"


void ledInit(void);
void ledSet(u16, u8);
void ledToggle(u16 led);
void ledUpdate(void);

#endif
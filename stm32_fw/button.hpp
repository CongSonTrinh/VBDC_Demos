#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "sys_config.h"

void buttonInit(void);
void buttonScan(void);
bool buttonIsPressed(u16 button, u32 start_cmp = 4);
bool buttonIsHold(u16 button, u32 start_cmp = 2);

#endif

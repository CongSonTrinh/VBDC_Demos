#ifndef TASK_HPP
#define TASK_HPP

#include "button.hpp"
#include "led.hpp"

void systemInit(void);
void taskHandleScreen(void);
void taskUpdateRtcTime(void);
void taskUpdateLed(void);
void taskUpdateTempAndHumid(void);
void taskProcessMessage(void);

#endif

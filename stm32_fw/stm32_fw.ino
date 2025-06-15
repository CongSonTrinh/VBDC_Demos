#include "task.hpp"

void setup() {
  Serial.begin(115200);
  systemInit();
}

void loop() {
  static unsigned long timer_tick = millis();
  while(millis() - timer_tick > TASK_INTERVALS);
  timer_tick = millis();
  buttonScan();
  taskHandleScreen();
}

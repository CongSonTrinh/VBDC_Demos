#include <sys/_stdint.h>
#include "uart.hpp"

#include "aht10.hpp"
#include "led.hpp"
#include "rtc.hpp"
#include "cmd.h"

static HardwareSerial CSerial(RXPIN, TXPIN);

String rtc2String()
{
  auto *rtcData = getRTCData();
  char buf[11];
  sprintf(buf, "%d%02d%02d%02d%02d%02d", 
    rtcData->dayOfWeek % 10,
    rtcData->date % 100,
    rtcData->month % 100,
    rtcData->year % 100,
    rtcData->hour % 100,
    rtcData->minute % 100);

  return String(buf);
}

String ledState2String()
{
  String s;
  s += (ledState(0) == LED_ON);
  s += (ledState(1) == LED_ON);
  s += (ledState(2) == LED_ON);
  s += (ledState(3) == LED_ON);

  return s;
}

String temp2String()
{
  float temp = tempAHT10();
  return (temp < 10.0 ? "0" : "") + String(temp, 2);
}

String humid2String()
{
  float humid = humidityAHT10();
  return (humid < 10.0 ? "0" : "") + String(humid, 2);
}

void initUart(void)
{
  CSerial.begin(230400);
}

void processMessage(void)
{
  if (!CSerial.available()) {
    return;
  }

  uint8_t cmd = CSerial.read();
  if (cmd == GET_DATA_CMD) {
    String s;
    s += rtc2String();
    s += ',';
    s += ledState2String();
    s += ',';
    s += temp2String();
    s += ',';
    s += humid2String();
    s += '\n';
    CSerial.print(s);
  } else if (cmd == SET_LEDS_CMD) {
    while(!CSerial.available());
    uint8_t led = CSerial.read();
    while(!CSerial.available());
    uint8_t isOn = CSerial.read();
    if (led < 4) {
      ledSet(led, isOn ? LED_ON : LED_OFF);
    }
  }
}

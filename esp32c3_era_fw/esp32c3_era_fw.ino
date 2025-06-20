#define ERA_LOCATION_VN
#define ERA_AUTH_TOKEN "4fc2a19d-e8bd-4720-97d3-d9702533a017"

#include <Arduino.h>
#include <ERa.hpp>
#include <Automation/ERaSmart.hpp>
#include <Time/ERaEspTime.hpp>

HardwareSerial CSerial(0);

SemaphoreHandle_t xCommSem;

#include "cmd.h"

const char ssid[] = "Son";
const char pass[] = "23456789";

ERaEspTime syncTime;
ERaSmart smart(ERa, syncTime);

#define P_VALUE(i)  \
  do { \
    int value = param.getInt(); \
    Serial.print("Value " #i " = "); \
    Serial.println(value); \
  } while(0)

String dayOfWeekString(const String& s)
{
  if (s == "1") return "Sun";
  else if (s == "2") return "Mon";
  else if (s == "3") return "Tue";
  else if (s == "4") return "Wed";
  else if (s == "5") return "Thu";
  else if (s == "6") return "Fri";
  else if (s == "7") return "Sat";
  else return "###";
}

String monthOfYearString(const String& s)
{
  if (s == "01") return "Jan";
  else if (s == "02") return "Feb";
  else if (s == "03") return "Mar";
  else if (s == "04") return "Apr";
  else if (s == "05") return "May";
  else if (s == "06") return "Jun";
  else if (s == "07") return "Jul";
  else if (s == "08") return "Aug";
  else if (s == "09") return "Sep";
  else if (s == "10") return "Oct";
  else if (s == "11") return "Nov";
  else if (s == "12") return "Dec";
  else return "###";
}

String time2String(String& s)
{
  String r;

  r += dayOfWeekString(s.substring(0, 1));
  r += ' ';
  r += s.substring(1, 3);
  r += ' ';
  r += monthOfYearString(s.substring(3, 5));
  r += ' ';
  r += String(2000 + s.substring(5, 7).toInt());
  r += ' ';
  r += s.substring(7, 9);
  r += ':';
  r += s.substring(9, 11);

  return r;
}

void setLedCmd(uint8_t led, uint8_t isOn)
{
  CSerial.write(SET_LEDS_CMD);
  CSerial.write(led);
  CSerial.write(isOn);
}

ERA_WRITE(V1) {
  uint8_t isOn = param.getInt();
  if (ERaOs::osSemaphoreAcquire(xCommSem, 1000) == osOK) {
    setLedCmd(0, isOn);
    ERaOs::osSemaphoreRelease(xCommSem);
  }
  ERa.virtualWrite(V1, isOn);
}

ERA_WRITE(V2) {
  uint8_t isOn = param.getInt();
  if (ERaOs::osSemaphoreAcquire(xCommSem, 1000) == osOK) {
    setLedCmd(1, isOn);
    ERaOs::osSemaphoreRelease(xCommSem);
  }
  ERa.virtualWrite(V2, isOn);
}

ERA_WRITE(V3) {
  uint8_t isOn = param.getInt();
  if (ERaOs::osSemaphoreAcquire(xCommSem, 1000) == osOK) {
    setLedCmd(2, isOn);
    ERaOs::osSemaphoreRelease(xCommSem);
  }
  ERa.virtualWrite(V3, isOn);
}

ERA_WRITE(V4) {
  uint8_t isOn = param.getInt();
  if (ERaOs::osSemaphoreAcquire(xCommSem, 1000) == osOK) {
    setLedCmd(3, isOn);
    ERaOs::osSemaphoreRelease(xCommSem);
  }
  ERa.virtualWrite(V4, isOn);
}

ERA_CONNECTED() {
  Serial.println("ERA Connected!");
}

ERA_DISCONNECTED() {
  Serial.println("ERA Disconnected!");
}

void timerEvent() {
  if (ERaOs::osSemaphoreAcquire(xCommSem, 1000) != osOK) {
    return;
  }

  CSerial.write(GET_DATA_CMD);
  String input = CSerial.readStringUntil('\n');

  ERaOs::osSemaphoreRelease(xCommSem);

  int pos1 = input.indexOf(',');
  int pos2 = input.indexOf(',', pos1 + 1);
  int pos3 = input.indexOf(',', pos2 + 1);

  String time   = input.substring(0, pos1);
  String led    = input.substring(pos1 + 1, pos2);
  String temp   = input.substring(pos2 + 1, pos3);
  String humid  = input.substring(pos3 + 1);

  ERa.virtualWrite(V0, time2String(time).c_str());
  ERa.virtualWrite(V1, (led[0] == '1'));
  ERa.virtualWrite(V2, (led[1] == '1'));
  ERa.virtualWrite(V3, (led[2] == '1'));
  ERa.virtualWrite(V4, (led[3] == '1'));
  ERa.virtualWrite(V5, temp.c_str());
  ERa.virtualWrite(V6, humid.c_str());
}

void setup() {
  Serial.begin(115200);
  CSerial.begin(230400, SERIAL_8N1, -1, -1);
  ERa.setScanWiFi(true);
  ERa.begin(ssid, pass);
  ERa.addInterval(200L, timerEvent);
  while(!Serial.isConnected());
#if defined(ERA_HAS_RTOS)
  Serial.println("ERA_HAS_RTOS");
  xCommSem = ERaOs::osSemaphoreNew();
  if (xCommSem == NULL)
  {
    Serial.println("Cannot create semaphore.");
    while(1);
  }
#endif
}

void loop() {
  ERa.run();
}

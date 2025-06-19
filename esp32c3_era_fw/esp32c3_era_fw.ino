#define ERA_LOCATION_VN
#define ERA_AUTH_TOKEN "4fc2a19d-e8bd-4720-97d3-d9702533a017"

#include <Arduino.h>
#include <ERa.hpp>
#include <Automation/ERaSmart.hpp>
#include <Time/ERaEspTime.hpp>

HardwareSerial CSerial(0);

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


void setLedCmd(uint8_t led, uint8_t isOn)
{
  CSerial.write(SET_LEDS_CMD);
  CSerial.write(led);
  CSerial.write(isOn);
}

ERA_WRITE(V1) {
  // P_VALUE(1);
  uint8_t isOn = param.getInt();
  setLedCmd(0, isOn);
}

ERA_WRITE(V2) {
  // P_VALUE(2);
  uint8_t isOn = param.getInt();
  setLedCmd(1, isOn);
}

ERA_WRITE(V3) {
  // P_VALUE(3);
  uint8_t isOn = param.getInt();
  setLedCmd(2, isOn);
}

ERA_WRITE(V4) {
  // P_VALUE(4);
  uint8_t isOn = param.getInt();
  setLedCmd(3, isOn);
}

// ERA_WRITE(V1) {

// }

ERA_CONNECTED() {
  Serial.println("ERA Connected!");
}

ERA_DISCONNECTED() {
  Serial.println("ERA Disconnected!");
}

void timerEvent() {
  // Serial.print("Send get data command, response: ");

  CSerial.write(GET_DATA_CMD);
  String input = CSerial.readStringUntil('\n');

  int pos1 = input.indexOf(',');
  int pos2 = input.indexOf(',', pos1 + 1);
  int pos3 = input.indexOf(',', pos2 + 1);

  String time   = input.substring(0, pos1);
  String led    = input.substring(pos1 + 1, pos2);
  String temp   = input.substring(pos2 + 1, pos3);
  String humid  = input.substring(pos3 + 1);

  // Serial.println(time);   // "12345"
  // Serial.println(led);    // "ON"
  // Serial.println(temp);   // "25"
  // Serial.println(humid);  // "80"

  ERa.virtualWrite(V0, time.c_str());
  ERa.virtualWrite(V1, !(led[0] == '1'));
  ERa.virtualWrite(V2, !(led[1] == '1'));
  ERa.virtualWrite(V3, !(led[2] == '1'));
  ERa.virtualWrite(V4, !(led[3] == '1'));
  ERa.virtualWrite(V5, temp.toFloat());
  ERa.virtualWrite(V6, humid.toFloat());
}

void setup() {
  Serial.begin(115200);
  CSerial.begin(115200, SERIAL_8N1, -1, -1);
  ERa.setScanWiFi(true);
  ERa.begin(ssid, pass);
  ERa.addInterval(1000L, timerEvent);
}

void loop() {
  ERa.run();
}

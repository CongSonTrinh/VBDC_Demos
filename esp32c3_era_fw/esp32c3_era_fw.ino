/*************************************************************
  Download latest ERa library here:
    https://github.com/eoh-jsc/era-lib/releases/latest
    https://www.arduino.cc/reference/en/libraries/era
    https://registry.platformio.org/libraries/eoh-ltd/ERa/installation

    ERa website:                https://e-ra.io
    ERa blog:                   https://iotasia.org
    ERa forum:                  https://forum.eoh.io
    Follow us:                  https://www.fb.com/EoHPlatform

 *************************************************************/
/* Select ERa host location (VN: Viet Nam, SG: Singapore) */
#define ERA_LOCATION_VN
#define ERA_AUTH_TOKEN "4fc2a19d-e8bd-4720-97d3-d9702533a017"

#include <Arduino.h>
#include <ERa.hpp>
#include <Automation/ERaSmart.hpp>
#include <Time/ERaEspTime.hpp>

const char ssid[] = "Son";
const char pass[] = "23456789";

ERaEspTime syncTime;
ERaSmart smart(ERa, syncTime);

HardwareSerial MCU_UART(0);

/* This function is called every time the Virtual Pin 0 state change */
ERA_WRITE(V0) {
    /* Get value from Virtual Pin 0 and write LED */
    uint8_t LedState = param.getInt() ? '1' : '0';
    MCU_UART.write(LedState);
}

/* This function will run every time ERa is connected */
ERA_CONNECTED() {
    // TODO: LED ON when connected
}

/* This function will run every time ERa is disconnected */
ERA_DISCONNECTED() {
    // TODO: LED OFF when connected
}

void timerEvent() {
    // T.B.D
}

void setup() {
    /* Setup debug console */
#if defined(ERA_DEBUG)
    Serial.begin(115200);
#endif
    MCU_UART.begin(115200, SERIAL_8N1, -1, -1);

    /* Setup pin mode led pin */
    pinMode(LED_PIN, OUTPUT);

    ERa.setScanWiFi(true);

    /* Initializing the ERa library. */
    ERa.begin(ssid, pass);

    /* Setup timer called function every second */
    ERa.addInterval(1000L, timerEvent);
}

void loop() {
    ERa.run();
}

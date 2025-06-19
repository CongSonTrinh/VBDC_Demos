#ifndef AHT10_HPP
#define AHT10_HPP

#include <Wire.h>

void initAHT10(TwoWire *wire = &Wire);
void updateAHT10(void);
bool hasNewDataAHT10(void);
float tempAHT10(void);
float humidityAHT10(void);

#endif

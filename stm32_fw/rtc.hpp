#ifndef MY_RTC_HPP
#define MY_RTC_HPP

#include <Wire.h>
#include <DS3231-RTC.h>

struct RtcData {
  uint8_t dayOfWeek{1};
  uint8_t date{1};
  uint8_t month{1};
  uint8_t year{1};
  uint8_t hour{1};
  uint8_t minute{1};
  uint8_t second{0};
  bool h12_mode{0};
  bool PM_time{0};
  bool century{0};
  float temperature{0.0f};
};

void rtcUpdate(void);
void initRtc(TwoWire *i2c = nullptr);
RtcData* getRTCData(void);
String dayOfWeekString(uint8_t);
String monthOfYearString(uint8_t);
inline bool isLeafYear(uint32_t);
int endOfMonth(uint8_t, uint8_t);

#endif
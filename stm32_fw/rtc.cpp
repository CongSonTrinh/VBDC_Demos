#include "rtc.hpp"

static DS3231 rtc;
static RtcData rtcData;

void initRtc(TwoWire *i2c)
{
  rtc._Wire = *i2c;
  rtcUpdate();
}

void rtcUpdate(void)
{
  rtcData.dayOfWeek   = rtc.getDoW();
  rtcData.date        = rtc.getDate();
  rtcData.month       = rtc.getMonth(rtcData.century);
  rtcData.year        = rtc.getYear();
  rtcData.hour        = rtc.getHour(rtcData.h12_mode, rtcData.PM_time);
  rtcData.minute      = rtc.getMinute();
  rtcData.second      = rtc.getSecond();
  rtcData.temperature = rtc.getTemperature();
}

RtcData* getRTCData()
{
  return &rtcData;
}

String dayOfWeekString(uint8_t doW)
{
  switch (doW) {
    case  1: return "Sun";
    case  2: return "Mon";
    case  3: return "Tue";
    case  4: return "Wed";
    case  5: return "Thu";
    case  6: return "Fri";
    case  7: return "Sat";
    default: return "###";
  }
}

String monthOfYearString(uint8_t month)
{
  switch(month) {
    case  1: return "Jan";
    case  2: return "Feb";
    case  3: return "Mar";
    case  4: return "Apr";
    case  5: return "May";
    case  6: return "Jun";
    case  7: return "Jul";
    case  8: return "Aug";
    case  9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "###";
  }
}

inline bool isLeafYear(uint32_t year)
{
  return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int endOfMonth(uint8_t month, uint8_t year)
{
  switch (month) {
    case 2:
      return isLeafYear(year) ? 29 : 28;
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return 31;
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;
    default:
      return -1;
  }
}

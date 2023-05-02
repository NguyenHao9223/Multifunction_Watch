#include "DS1307.h"

RTC_DS1307 rtc;

byte _day, _month, _hour24, _hour12, _minute, _second, _dtw;
int _year;

byte hr24;
char st[3];

void SetupDS1307() {
  Serial.begin(115200);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

}

void GetDateTime() {
  DateTime now = rtc.now();
  _day = now.day();
  _month = now.month();
  _year = now.year();
  _hour24 = now.hour();
  _minute = now.minute();
  _second = now.second();
  _dtw = now.dayOfTheWeek();

  if (_hour24 >= 12) {
    strncpy(st, "PM\0", sizeof(st));
    if (_hour24 > 12) {
      _hour12 = _hour24 - 12;
    } else {
      _hour12 = 12;
    }
  } else {
    strncpy(st, "AM\0", sizeof(st));
    if (_hour24 == 0) {
      _hour12 = 12;
    } else {
      _hour12 = _hour24;
    }
  }
}

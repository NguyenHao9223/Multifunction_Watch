#ifndef DS1307_h
#define DS1307_h

#include "RTClib.h"

extern RTC_DS1307 rtc;

extern byte _day, _month, _hour24, _hour12, _minute, _second, _dtw;
extern int _year;
extern byte hr24;
extern char st[3];

void SetupDS1307();
void GetDateTime();

#endif

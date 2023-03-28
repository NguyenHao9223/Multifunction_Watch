#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "RTClib.h"
#include <DHT.h>


#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

RTC_DS1307 rtc;

#define OLED_RESET     4 
Adafruit_SH1106 display(OLED_RESET);

#define DHTPIN 2    
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
const float NHIET_DO_CAO = 35.0; //Khai bao 1 bien nhiet do cao = 35 do C
byte Buzzer = 7;

byte _day, _month, _hour24, _hour12, _minute, _second, _dtw;
int _year;  //Biến _year được sử dụng để lưu trữ giá trị của năm, và giá trị năm có thể lớn hơn 255 (giá trị tối đa mà một biến byte có thể lưu trữ).
            // Vì vậy, để đảm bảo rằng biến có thể lưu trữ tất cả các giá trị năm, biến _year được định nghĩa là kiểu int, có thể lưu trữ giá trị từ -32768 đến 32767.
byte hr24;
char st[3];
char _nameoftheDay[9];
char _monthsName[3];

//định nghĩa một mảng các chuỗi ký tự (hay còn gọi là mảng các strings) để lưu trữ tên của các ngày trong tuần.
const char daysOfTheWeek_0[] PROGMEM = "Sunday";   
const char daysOfTheWeek_1[] PROGMEM = "Monday";
const char daysOfTheWeek_2[] PROGMEM = "Tuesday";
const char daysOfTheWeek_3[] PROGMEM = "Wednesday";
const char daysOfTheWeek_4[] PROGMEM = "Thursday";
const char daysOfTheWeek_5[] PROGMEM = "Friday";
const char daysOfTheWeek_6[] PROGMEM = "Saturday";

const char* const daysOfTheWeek_table[] PROGMEM = {daysOfTheWeek_0, daysOfTheWeek_1, daysOfTheWeek_2, daysOfTheWeek_3, daysOfTheWeek_4, daysOfTheWeek_5, daysOfTheWeek_6};
//const char* const, có chứa các con trỏ (pointers) tới các chuỗi ký tự được lưu trữ trong các mảng daysOfTheWeek_X.
//Mảng này cũng được đánh dấu là PROGMEM để lưu trữ trong bộ nhớ flash của vi điều khiển, giúp giảm tải cho bộ nhớ RAM của vi điều khiển.

const char months_name_0[] PROGMEM = "Jan";   
const char months_name_1[] PROGMEM = "Feb";
const char months_name_2[] PROGMEM = "Mar";
const char months_name_3[] PROGMEM = "Apr";
const char months_name_4[] PROGMEM = "May";
const char months_name_5[] PROGMEM = "Jun";
const char months_name_6[] PROGMEM = "Jul";
const char months_name_7[] PROGMEM = "Aug";
const char months_name_8[] PROGMEM = "Sep";
const char months_name_9[] PROGMEM = "Oct";
const char months_name_10[] PROGMEM = "Nov";
const char months_name_11[] PROGMEM = "Dec";

const char* const months_name_table[] PROGMEM = {months_name_0, months_name_1, months_name_2, months_name_3, 
                                                 months_name_4, months_name_5, months_name_6, months_name_7,
                                                 months_name_8, months_name_9, months_name_10, months_name_11,};

void setup() {
  Serial.begin(115200);
    pinMode(Buzzer, OUTPUT);

    if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  }

  dht.begin();

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
}
void loop() {
  display.clearDisplay();
 
  GetDateTime(); 
  display.clearDisplay(); 
  Digital_Clock_and_Date(_hour12, _minute, _second, _dtw, _day, _month ,_year, st); 
  displayTempHumid();
 
  display.display();
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
void Digital_Clock_and_Date(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr, char* st) {
  display.setTextColor(WHITE);
  display.setCursor(35, 0);
  if (h < 10) {
    display.print("0");
    display.print(h);
  } else {
    display.print(h);
  }
  display.print(":");
  if (m < 10) {
    display.print("0");
    display.print(m);
  } else {
    display.print(m);
  }
  display.print(":");
  if (s < 10) {
    display.print("0");
    display.print(s);
  } else {
    display.print(s);
  }    
  display.print(" ");
  display.print(st);

  strcpy_P(_nameoftheDay,(char*)pgm_read_word(&(daysOfTheWeek_table[dtw])));//Để tiết kiệm bộ nhớ RAM có thể lưu trữ dữ liệu này trong bộ nhớ flash và truy cập dữ liệu đó bằng lệnh pgm_read_*
  display.setCursor(0, 10);
  display.println(_nameoftheDay); 

  strcpy_P(_monthsName,(char*)pgm_read_word(&(months_name_table[mt-1])));
  display.setCursor(50, 10);
  if (dy < 10) {
    display.print("0");
    display.print(dy);
  } else {
    display.print(dy);
  }
  display.print(", ");
  display.print(_monthsName);
  display.setCursor(95,10);
  display.print(yr);
}
void displayTempHumid(){
  float temperature = dht.readTemperature();  // Đọc giá trị nhiệt độ từ DHT11
  float humidity = dht.readHumidity();  // Đọc giá trị độ ẩm từ DHT11

  display.setCursor(0, 40);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");

  display.setCursor(0, 50);
  display.print("Hum: ");
  display.print(humidity);
  display.print(" %");
  if (temperature > NHIET_DO_CAO) {
    Button_Sound(1); // Bật buzzer
    delay(10000); // Giữ buzzer kêu trong 10 giây
    Button_Sound(0); // Tắt buzzer
  }
}
void Button_Sound(byte snd) { //điều khiển đầu ra âm thanh của buzzer bằng cách sử dụng hàm digitalWrite(). Tham số snd cho biết trạng thái của buzzer: 1 để bật và 0 để tắt.
  digitalWrite(Buzzer, snd);
}
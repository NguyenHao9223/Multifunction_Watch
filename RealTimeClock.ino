#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"

RTC_DS1307 rtc;

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte _day, _month, _hour24, _hour12, _minute, _second, _dtw;
int _year;
byte hr24;
char st[2];
char _nameoftheDay[9];
char _monthsName[3];

const char daysOfTheWeek_0[] PROGMEM = "Sunday";   
const char daysOfTheWeek_1[] PROGMEM = "Monday";
const char daysOfTheWeek_2[] PROGMEM = "Tuesday";
const char daysOfTheWeek_3[] PROGMEM = "Wednesday";
const char daysOfTheWeek_4[] PROGMEM = "Thursday";
const char daysOfTheWeek_5[] PROGMEM = "Friday";
const char daysOfTheWeek_6[] PROGMEM = "Saturday";

const char* const daysOfTheWeek_table[] PROGMEM = {daysOfTheWeek_0, daysOfTheWeek_1, daysOfTheWeek_2, daysOfTheWeek_3, daysOfTheWeek_4, daysOfTheWeek_5, daysOfTheWeek_6};

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

byte clockCenterX=31;   //Toa do tam dong ho tren man hinh
byte clockCenterY=31;

const unsigned char Clock_Frame [] PROGMEM = {
  0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x1f, 0xe0, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0xf0, 0x18, 0x70, 0x1e, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x28, 0x88, 0x07, 0x80, 0x00, 
  0x00, 0x07, 0x00, 0x08, 0x08, 0x01, 0xc0, 0x00, 0x00, 0x1c, 0x00, 0x08, 0x10, 0x00, 0x70, 0x00, 
  0x00, 0x38, 0x20, 0x08, 0x20, 0x10, 0x38, 0x00, 0x00, 0x70, 0x20, 0x08, 0x40, 0x10, 0x1c, 0x00, 
  0x00, 0xe0, 0x10, 0x3e, 0xf8, 0x20, 0x0e, 0x00, 0x01, 0xc0, 0x10, 0x00, 0x00, 0x20, 0x07, 0x00, 
  0x01, 0x80, 0x08, 0x00, 0x00, 0x40, 0x03, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00, 0x40, 0x01, 0x80, 
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x30, 0x38, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 
  0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0x30, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 
  0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0xc7, 0x00, 0x00, 0x03, 0x80, 0x00, 0x01, 0xc6, 0xc8, 0x80, 0x00, 0x07, 0xc0, 0x00, 0x02, 0x26, 
  0xc8, 0x80, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x26, 0xc7, 0x80, 0x00, 0x0c, 0x60, 0x00, 0x00, 0xc6, 
  0xc0, 0x80, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x26, 0xc1, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x02, 0x26, 
  0xce, 0x00, 0x00, 0x03, 0x80, 0x00, 0x01, 0xc6, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 
  0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 
  0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x30, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
  0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0x38, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 
  0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x30, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x08, 0x00, 0x00, 0x20, 0x01, 0x80, 
  0x01, 0x80, 0x08, 0x00, 0x00, 0x20, 0x03, 0x00, 0x01, 0xc0, 0x10, 0x00, 0x00, 0x10, 0x07, 0x00, 
  0x00, 0xe0, 0x10, 0x03, 0x80, 0x10, 0x0e, 0x00, 0x00, 0x70, 0x20, 0x06, 0x00, 0x08, 0x1c, 0x00, 
  0x00, 0x38, 0x20, 0x04, 0x00, 0x08, 0x38, 0x00, 0x00, 0x1c, 0x00, 0x07, 0x80, 0x00, 0x70, 0x00, 
  0x00, 0x07, 0x00, 0x04, 0x40, 0x01, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x04, 0x40, 0x07, 0x80, 0x00, 
  0x00, 0x00, 0xf0, 0x03, 0x80, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0x1f, 0xe0, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00
};


void setup() {
  Serial.begin(9600); 
  if (!rtc.begin()) {  
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!rtc.isrunning()) {  
    Serial.println("RTC is NOT running!");
    // Khởi tạo RTC nếu chưa được chạy
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {

  display.clearDisplay();
  DateTime now = rtc.now(); 

  GetDateTime(); 
  display.clearDisplay(); 
  Draw_Clock_Frame(); 
  Analog_Clock(_hour12,_minute,_second);
  Digital_Clock_and_Date(_hour12, _minute, _second, _dtw, _day, _month ,_year); 
  display.display();
}

void GetDateTime(){ 
  DateTime now = rtc.now(); 
  _day=now.day(); 
  _month=now.month(); 
  _year=now.year(); 
  _hour24=now.hour(); 
  _minute=now.minute(); 
  _second=now.second();
  _dtw=now.dayOfTheWeek();  

  hr24=_hour24; 
  if (hr24>12) {
    _hour12=hr24-12; 
  }
  else if (hr24==0) { 
    _hour12=12;
  }
  else { 
    _hour12=hr24;
  }

  if (hr24<12) {  
    strncpy(st, "AM", sizeof(st));
  }
  else {
    strncpy(st, "PM", sizeof(st));;
  }  
}

void Draw_Clock_Frame() {
  display.drawBitmap(0, 0, Clock_Frame, 63, 63, WHITE); 
}

void Analog_Clock(byte h, byte m, byte s) { //vẽ một đồng hồ kim loại trên màn hình hiển thị OLED. Các tham số h, m, và s được truyền vào hàm lần lượt đại diện cho giờ, phút và giây của thời gian hiện tại.
  Draw_Line_Seconds(s); //Đây là một hàm khác được gọi để vẽ kim giây tại vị trí phù hợp trên đồng hồ analog dựa trên tham số s truyền vào.
  Draw_Line_Minutes(m); //Hàm này được sử dụng để vẽ kim phút trên đồng hồ analog dựa trên tham số m.
  Draw_Line_Hours(h,m); //Đây là hàm để vẽ kim giờ trên đồng hồ analog dựa trên tham số h và m.
  
  for (byte i = 30; i < 33; i++) {    //vòng lặp for để vẽ một chấm tròn nhỏ ở giữa đồng hồ. Vòng lặp sẽ chạy 3 lần (i từ 30 đến 32) để vẽ các pixel tạo thành chấm tròn tại các tọa độ xác định trên màn hình sử dụng phương thức drawPixel() của thư viện Adafruit_SSD1306.
    display.drawPixel(i, 30, SSD1306_BLACK);
    display.drawPixel(i, 31, SSD1306_BLACK);
    display.drawPixel(i, 32, SSD1306_BLACK);
  }
}
void Draw_Line_Seconds(int s) { //vẽ kim giây trên đồng hồ analog.
  float angle = s * 6 ; //angle là góc quay của kim giây, tính bằng cách nhân số giây hiện tại với 6. Vì một vòng tròn là 360 độ và một phút có 60 giây, vì vậy mỗi giây sẽ tương ứng với 6 độ.
  angle = (angle / 57.29577951);  //Chuyển đổi đơn vị góc từ độ sang radian.
  int xS = (clockCenterX + (sin(angle) * (16)));  //xS và yS là tọa độ của kim giây trên đồng hồ. clockCenterX và clockCenterY là tọa độ của trung tâm đồng hồ trên màn hình hiển thị. 
  int yS = (clockCenterY - (cos(angle) * (16)));  //Hàm sin() và cos() được sử dụng để tính toán tọa độ x và y của kim giây trên đường tròn, trong đó bán kính là 16 pixel.
  if (s == 15) {  //Kiểm tra nếu giây hiện tại là 15, thì vẽ một đường thẳng dài hơn một pixel để tạo ra một mũi tên cho kim giây.
    display.drawLine(clockCenterX, clockCenterY, xS, yS + 1, WHITE);  //vẽ một đường thẳng từ tọa độ trung tâm đồng hồ đến tọa độ của kim giây (xS, yS). Màu của đường được chọn là màu trắng (WHITE).
  } else {
    display.drawLine(clockCenterX, clockCenterY, xS, yS, WHITE);
  }
  }

  void Draw_Line_Minutes(int m) {
  float angle = m * 6 ;
  angle = (angle / 57.29577951) ;  

  int xM = (clockCenterX + (sin(angle) * (17)));
  int yM = (clockCenterY - (cos(angle) * (17)));

  if (m == 15) {
    display.drawLine(clockCenterX, clockCenterY - 1, xM, yM + 1, WHITE);
    display.drawLine(clockCenterX - 1, clockCenterY, xM, yM + 1, WHITE);
    display.drawLine(clockCenterX, clockCenterY, xM, yM + 1, WHITE);
    display.drawLine(clockCenterX + 1, clockCenterY, xM, yM + 1, WHITE);
    display.drawLine(clockCenterX, clockCenterY + 1, xM, yM + 1, WHITE);
  } else {
    display.drawLine(clockCenterX, clockCenterY - 1, xM, yM, WHITE);
    display.drawLine(clockCenterX - 1, clockCenterY, xM, yM, WHITE);
    display.drawLine(clockCenterX, clockCenterY, xM, yM, WHITE);
    display.drawLine(clockCenterX + 1, clockCenterY, xM, yM, WHITE);
    display.drawLine(clockCenterX, clockCenterY + 1, xM, yM, WHITE);
  }
}
void Draw_Line_Hours(int h, int m) {
  float angle = h * 30 + int((m / 12) * 6);
  angle = (angle / 57.29577951) ;

  int xH = (clockCenterX + (sin(angle) * (12)));
  int yH = (clockCenterY - (cos(angle) * (12)));
  if (h == 12 && m < 12) {
    display.drawLine(clockCenterX, clockCenterY - 2, xH + 1, yH, WHITE);
    display.drawLine(clockCenterX - 2, clockCenterY, xH + 1, yH, WHITE);    
    display.drawLine(clockCenterX, clockCenterY - 1, xH + 1, yH, WHITE);
    display.drawLine(clockCenterX - 1, clockCenterY, xH + 1, yH, WHITE);
    display.drawLine(clockCenterX, clockCenterY, xH + 1, yH, WHITE);
    display.drawLine(clockCenterX + 1 ,clockCenterY, xH + 1, yH, WHITE);
    display.drawLine(clockCenterX, clockCenterY + 1, xH + 1, yH, WHITE);
    display.drawLine(clockCenterX + 2, clockCenterY, xH + 1, yH ,WHITE);
    display.drawLine(clockCenterX, clockCenterY + 2, xH + 1, yH, WHITE);
  } else {
    display.drawLine(clockCenterX, clockCenterY - 2, xH, yH, WHITE);
    display.drawLine(clockCenterX - 2, clockCenterY, xH, yH, WHITE);    
    display.drawLine(clockCenterX, clockCenterY - 1, xH, yH, WHITE);
    display.drawLine(clockCenterX - 1, clockCenterY, xH, yH, WHITE);
    display.drawLine(clockCenterX, clockCenterY, xH, yH, WHITE);
    display.drawLine(clockCenterX + 1 ,clockCenterY, xH, yH, WHITE);
    display.drawLine(clockCenterX, clockCenterY + 1, xH, yH, WHITE);
    display.drawLine(clockCenterX + 2, clockCenterY, xH, yH ,WHITE);
    display.drawLine(clockCenterX, clockCenterY + 2, xH, yH, WHITE);
  }
}

void Digital_Clock_and_Date(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(73, 0);
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

  strcpy_P(_monthsName,(char*)pgm_read_word(&(months_name_table[mt-1])));
  display.setCursor(76, 15);
  if (dy < 10) {
    display.print("0");
    display.print(dy);
  } else {
    display.print(dy);
  }
  display.print(", ");
  display.print(_monthsName);
  display.setCursor(85, 25);
  display.print(yr);
}
  
#include <Adafruit_GFX.h>
#include "DS1307.h"
#include "DHT11.h"
#include <EEPROM.h>

#define USE_LCD 0x1106
#include <Adafruit_SH1106_kbv.h>
Adafruit_SH1106_kbv display(128, 64, &Wire);
#define LCD_BEGIN()    display.begin(SSD1306_SWITCHCAPVCC, 0x3C)

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 

byte btn_Menu_Select, btn_Up, btn_Down; 
byte Buzzer = 7; 

byte clockCenterX=31;
byte clockCenterY=31;

//BIT MAP CLOCK FRAME################################################################################################################################################################
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

const unsigned char Select_Top [] PROGMEM = {
  0x20, 0x70, 0xf8
};

// 'Select_Side', 3x5px
const unsigned char Select_Side [] PROGMEM = {
  0x80, 0xc0, 0xe0, 0xc0, 0x80
};

// 'Select_Buttom', 5x3px
const unsigned char Select_Buttom [] PROGMEM = {
  0xf8, 0x70, 0x20
};

unsigned long previousMillisGetTimeDate = 0; //kiểu unsigned long và gán giá trị ban đầu là 0. Biến này được sử dụng để lưu trữ thời gian trước đó khi lấy thời gian và ngày tháng.
const long intervalGetTimeDate = 1000;       //đặt khoảng thời gian giữa các lần lấy thời gian và ngày tháng.  
bool Menu_Stat = false;
byte Menu_Cnt = 0;
bool Menu_Set = false;
byte Menu_Set_cnt;
byte Menu_Set_TimeDate = 0;

bool set_Hour = false;
bool set_Minute = false;
bool set_Day = false;
bool set_Month = false;
bool set_Year = false;

bool blink_cursor = false;

byte Alarm_Stat = 0;                //khởi tạo với giá trị 0, sẽ được sử dụng để theo dõi trạng thái của báo thức (on/off).
byte hour_alarm, minute_alarm;      //sử dụng để lưu giờ và phút được thiết lập cho báo thức.
byte Menu_Set_Alarm = 0;            //sử dụng để theo dõi trạng thái hiện tại của menu thiết lập báo thức.
byte Alarm_Duration = 0;            //sử dụng để lưu thời gian chạy của báo thức.
#define Alarm_Stat_address    0     //địa chỉ lưu trữ EEPROM 
#define hour_alarm_address    1
#define minute_alarm_address  2
bool Set_hour_alarm = false;        //theo dõi các trạng thái khác nhau của báo thức. 
bool Set_minute_alarm = false;
bool Alarm_Sound = false;
bool Alarm_Start = false;
bool Alarm_Trig = true;
bool Blink_Alarm_Display = false;
int _delay = 100;

// HÀM SETUP**********************************************************************************************
//********************************************************************************************************
void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);

  SetupDS1307();
  DHT11_setup();
  LCD_BEGIN(); 
  Alarm_Stat = EEPROM.read(Alarm_Stat_address);     //đọc giá trị được lưu trữ trong EEPROM tại địa chỉ được chỉ định bởi hằng số Alarm_Stat_address và gán nó cho biến Alarm_Stat. Biến này sẽ được sử dụng để theo dõi trạng thái của báo thuc.
  delay(100);                                       //delay de đảm bảo rằng các giá trị đọc được từ EEPROM không bị nhiễu bởi các hoạt động khác trong chương trình.
  hour_alarm = EEPROM.read(hour_alarm_address);
  delay(100);
  minute_alarm = EEPROM.read(minute_alarm_address); 
  delay(100);
}

//HÀM LOOP************************************************************************************************
//********************************************************************************************************
void loop() {
  unsigned long currentMillisGetTimeDate = millis();                                   //sử dụng hàm millis() để lấy thời gian hiện tại và gán giá trị cho biến currentMillisGetTimeDate
  if (currentMillisGetTimeDate - previousMillisGetTimeDate >= intervalGetTimeDate) {   //kiểm tra xem đã đến thời điểm lấy thời gian và ngày tháng hay chưa. Nếu thời gian đã trôi qua khoảng thời gian được đặt bởi hằng số intervalGetTimeDate.
    previousMillisGetTimeDate = currentMillisGetTimeDate;                              //cập nhật giá trị của biến previousMillisGetTimeDate bằng currentMillisGetTimeDate
    if (Menu_Stat == false) {                                                          // bnt menu_stat không nhấn tức là False. Nếu đúng, thực hiện các lệnh để lấy thời gian và ngày tháng từ hàm GetDateTime()
      GetDateTime();                                                                   // xóa màn hình OLED bằng hàm display.clearDisplay() và hiển thị thời gian và ngày tháng trên màn hình OLED bằng hàm Digital_Clock_and_Date()
      DHT11_read();
      display.clearDisplay();
      Draw_Clock_Frame();
      Analog_Clock(_hour12,_minute,_second);

      //DIEU KIEN DE BAT DAU BAO THUC
      if (Alarm_Stat == 1 && _hour24 == hour_alarm && _minute == minute_alarm && Alarm_Trig == true) {
      //Nếu trạng thái báo động (Alarm_Stat) bằng 1 và giờ, phút hiện tại (_hour24 và _minute) bằng với giờ, phút được thiết lập (hour_alarm và minute_alarm) và Alarm_Trig là true thi.
        Alarm_Start = true;
        Alarm_Trig = false;
      }

      if (_minute != minute_alarm) {    //Nếu phút hiện tại khác với phút được thiết lập thì Alarm_Trig sẽ được thiết lập lại thành true,
        Alarm_Trig = true;              // để chu kỳ kích hoạt báo động tiếp theo bắt đầu từ lúc phút được thiết lập.
      }

      //DIEU KIEN DE THUC THI BAO THUC VA DUNG BAO THUC
      if (Alarm_Start == true) {        //Nếu báo động được kích hoạt (Alarm_Start = true), biến Alarm_Sound sẽ đổi giá trị (true/false) để thực hiện chế độ phat am thanh cho báo thuc.
        Alarm_Sound = !Alarm_Sound;
        if (Alarm_Sound == true) {
          Button_Sound(0);
        } else {
          Button_Sound(1);
        }
        Alarm_Duration++;           //Biến Alarm_Duration sẽ tăng thêm 1, tượng trưng cho thời gian báo thức. Nếu thời gian báo thức đã đạt giá trị 59, báo thức sẽ được dừng lại.
        if (Alarm_Duration > 59) {
          Button_Sound(0);
          Alarm_Duration = 0;       //Biến Alarm_Start, Alarm_Trig và Alarm_Duration sẽ được thiết lập về lại giá trị ban đầu.
          Alarm_Start = false;
        }

        display.setTextSize(1);
        Blink_Alarm_Display = !Blink_Alarm_Display;   //"Blink_Alarm_Display" là true, màn hình sẽ hiển thị chữ và nền trắng, ngược lại sẽ hiển thị chữ và nền đen.
        if (Blink_Alarm_Display == true) {
          display.setTextColor(WHITE);
        } else {
          display.setTextColor(BLACK);
        }      
        display.setCursor(82, 22);
        display.print("Alarm");
        display.setCursor(82, 32);
        if (hour_alarm < 10) {
          display.print("0");
          display.print(hour_alarm);
        } else {
          display.print(hour_alarm);
        }
        display.print(":");
        if (minute_alarm < 10) {
          display.print("0");
          display.print(minute_alarm);
        } else {
          display.print(minute_alarm);
        }
      } else {

      Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);
      DHT11_display();
      }
      display.display();  //hiển thị nội dung đã được cập nhật trên màn hình OLED.
    }
  }
  read_button();  //Đọc nút nhấn
  check_button(); //Kiểm tra trạng thái nút nhấn
}


//HÀM HIỂN THỊ THÔNG TIN CỦA NHIỆT ĐỘ VÀ ĐỘ ẨM************************************************************
//********************************************************************************************************
void DHT11_display(){
  if (isnan(doam) || isnan(doC)) 
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(70, 45);
    display.println("Error to ");
    display.setCursor(60, 55);
    display.println("read DHT11!");
    return;
  }
  display.setCursor(70, 45);
  display.print("T:");
  display.print(doC); 
  display.print("C");
  display.setCursor(70, 55);
  display.print("H:");
  display.print(doam);
  display.print("%");  

  if (doC > 35) {
    // Xóa màn hình tại vị trí (70, 55)
    display.fillRect(70, 55, display.width() - 70, 10, BLACK);
    
    // In dòng "High temp" tại vị trí (0, 45)
    display.setCursor(65, 55);
    display.setTextColor(WHITE);
    // display.print("High temp!");
    Button_Sound(0);       
    delay(100);
    // display.fillRect(65, 55, display.width() - 65, 10, BLACK);
    Button_Sound(1);
  }
}

//HÀM ĐỌC NÚT NHẤN****************************************************************************************
//********************************************************************************************************
void read_button() {
  btn_Menu_Select = digitalRead(4);
  btn_Up = digitalRead(5);
  btn_Down = digitalRead(6);
}

//HÀM KIỂM TRA TRẠNG THÁI NÚT NHẤN************************************************************************
//********************************************************************************************************
void check_button() {
  if (btn_Menu_Select == HIGH && Menu_Stat == false) { //Nếu nút btn_Menu_Select được nhấn và trạng thái Menu_Stat là false.
    Button_Sound(0);  //Phát ra âm thanh để cho biết nút đã được nhấn.
    delay(500);      
    Button_Sound(1);  //Tắt âm thanh
    Menu_Cnt = 1;     //Thiết lập biến Menu_Cnt lên 1.
    Menu_Stat = true; //Thiết lập lại trạng thái Menu_Stat là true.
  } 
  while (Menu_Stat == true) { //Lặp lại cho đến khi Menu_Stat là false.
    read_button();            //Đọc trạng thái các nút nhấn. 
    if (btn_Down == HIGH) {    //nút btn_Down được nhấn.
      Button_Sound(0);        //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(100);
      Button_Sound(1);        //Tắt âm thanh.
      Menu_Cnt++;             //Tăng giá trị của Menu_Cnt lên 1.
      if (Menu_Cnt > 3) Menu_Cnt = 1; //Nếu giá trị của Menu_Cnt vượt quá 3 thì thiết lập nó về 1.
    }
  
    if (btn_Up == HIGH) {      //nút btn_Up được nhấn.
      Button_Sound(0);        //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(100);
      Button_Sound(1);        //Tắt âm thanh.
      Menu_Cnt--;             //Giảm giá trị của Menu_Cnt lên 1.
      if (Menu_Cnt < 1) Menu_Cnt = 3; //Nếu giá trị của Menu_Cnt vượt quá 1 thì thiết lập nó về 3.
    }

    if (btn_Menu_Select == HIGH && Menu_Cnt == 1) { //Nếu bnt_Menu_Select được nhấn và Menu_Cnt ==1
      Button_Sound(0);  //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(500);
      Button_Sound(1);  //Tắt âm thanh.
      Menu_Set = true;  //Thiết lập lại trạng thái Menu_Set là true.
      Menu_Set_cnt = 1; //Thiết lập lại trạng thái Menu_Set_Cnt là 1.  
    }
  
    if (btn_Menu_Select == HIGH && Menu_Cnt == 2) { //Nếu bnt_Menu_Select được nhấn và Menu_Cnt ==2
      Button_Sound(1);  //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(500);
      Button_Sound(0);  //Tắt âm thanh.
      Menu_Set = true;  //Thiết lập lại trạng thái Menu_Set là true.
      Menu_Set_cnt = 2; //Thiết lập lại trạng thái Menu_Set_Cnt là 2. 
      delay(100);
    }

    if (btn_Menu_Select == HIGH && Menu_Cnt == 3) {  //Nếu bnt_Menu_Select được nhấn và Menu_Cnt ==3
      Button_Sound(1); //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(500);
      Button_Sound(0);  //Tắt âm thanh.
      Menu_Cnt = 1;     //Thiết lập biến Menu_Cnt về 1.
      Menu_Stat = false;  //Thiết lập lại trạng thái Menu_Stat là false. -> Tắt menu, hiển thị thông tin ngày giờ... 
    }
    if (Menu_Cnt == 1) {
      Menu_Display(Menu_Cnt);//hiển thị menu tương ứng.
    } else if (Menu_Cnt == 2) {
      Menu_Display(Menu_Cnt);//hiển thị menu tương ứng.
    } if (Menu_Cnt == 3) {
      Menu_Display(Menu_Cnt);//hiển thị menu tương ứng.
    } 
    while (Menu_Set == true && Menu_Set_cnt == 1) { //Nếu Menu_Set là true và Menu_Set_cnt = 1 thì sẽ nhảy vào chương trình cài đặt ngày giờ.
      Set_Time_Date();
      delay(50);
    }
    while (Menu_Set == true && Menu_Set_cnt == 2) { //Nếu Menu_Set là true và Menu_Set_cnt = 2 thì sẽ nhảy vào chương trình cài đặt ngày giờ.
      Set_Alarm();
      delay(50);
    }
    delay(50);
  }
}

//******MẢNG LƯU TÊN BIẾN THỜI GIAN*********************************************************************************************************************************************
char _nameoftheDay[9];                                                                                                                                                       //*
char _monthsName[3];                                                                                                                                                         //*
                                                                                                                                                                             //*
const char daysOfTheWeek_0[] PROGMEM = "Sunday";                                                                                                                             //*
const char daysOfTheWeek_1[] PROGMEM = "Monday";                                                                                                                             //*
const char daysOfTheWeek_2[] PROGMEM = "Tuesday";                                                                                                                            //*
const char daysOfTheWeek_3[] PROGMEM = "Wednesday";                                                                                                                          //*
const char daysOfTheWeek_4[] PROGMEM = "Thursday";                                                                                                                           //*
const char daysOfTheWeek_5[] PROGMEM = "Friday";                                                                                                                             //*
const char daysOfTheWeek_6[] PROGMEM = "Saturday";                                                                                                                           //*
                                                                                                                                                                             //*
const char* const daysOfTheWeek_table[] PROGMEM = {daysOfTheWeek_0, daysOfTheWeek_1, daysOfTheWeek_2, daysOfTheWeek_3, daysOfTheWeek_4, daysOfTheWeek_5, daysOfTheWeek_6};   //*
                                                                                                                                                                             //*
const char months_name_0[] PROGMEM = "Jan";                                                                                                                                  //*
const char months_name_1[] PROGMEM = "Feb";                                                                                                                                  //*
const char months_name_2[] PROGMEM = "Mar";                                                                                                                                  //*
const char months_name_3[] PROGMEM = "Apr";                                                                                                                                  //*
const char months_name_4[] PROGMEM = "May";                                                                                                                                  //*
const char months_name_5[] PROGMEM = "Jun";                                                                                                                                  //*
const char months_name_6[] PROGMEM = "Jul";                                                                                                                                  //*
const char months_name_7[] PROGMEM = "Aug";                                                                                                                                  //*
const char months_name_8[] PROGMEM = "Sep";                                                                                                                                  //*
const char months_name_9[] PROGMEM = "Oct";                                                                                                                                  //*
const char months_name_10[] PROGMEM = "Nov";                                                                                                                                 //*
const char months_name_11[] PROGMEM = "Dec";                                                                                                                                 //*
                                                                                                                                                                             //*
const char* const months_name_table[] PROGMEM = {months_name_0, months_name_1, months_name_2, months_name_3,                                                                 //*
                                                 months_name_4, months_name_5, months_name_6, months_name_7,                                                                 //*
                                                 months_name_8, months_name_9, months_name_10, months_name_11,};                                                             //* 
//******************************************************************************************************************************************************************************                                                 

//HÀM HIỂN THỊ THÔNG TIN VỀ THỜI GIAN*********************************************************************
//********************************************************************************************************                                                 
void Digital_Clock_and_Date(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr) {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // display.setCursor(73, 0);
  // if (h < 10) {
  //   display.print("0");
  //   display.print(h);
  // } else {
  //   display.print(h);
  // }
  // display.print(":");
  // if (m < 10) {
  //   display.print("0");
  //   display.print(m);
  // } else {
  //   display.print(m);
  // }
  // display.print(":");
  // if (s < 10) {
  //   display.print("0");
  //   display.print(s);
  // } else {
  //   display.print(s);
  // }    
  // //----------------------------------------

 if (Menu_Stat == false) {
    strcpy_P(_nameoftheDay,(char*)pgm_read_word(&(daysOfTheWeek_table[dtw])));
    byte display_center_area = (127 - 65) / 2;
    byte center_text = (strlen(_nameoftheDay) * 6) / 2;
    byte x_text = 66 + (display_center_area - center_text);
    display.setCursor(x_text, 0);
    display.println(_nameoftheDay); 
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
  display.setCursor(85, 30);
  display.print(yr);
  //----------------------------------------
}

//HÀM HIỂN THỊ TRONG CÀI ĐẶT THỜI GIAN
void Digital_Clock_and_Dateee(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr) {
  display.setTextSize(1);
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
  strcpy_P(_nameoftheDay,(char*)pgm_read_word(&(daysOfTheWeek_table[dtw])));
  display.setCursor(0, 15);
  display.println(_nameoftheDay); 

  strcpy_P(_monthsName,(char*)pgm_read_word(&(months_name_table[mt-1])));
  display.setCursor(60, 15);
  if (dy < 10) {
    display.print("0");
    display.print(dy);
  } else {
    display.print(dy);
  }
  display.print(",");
  display.print(_monthsName);
  display.print(" ");
  display.print(yr);
}

//HÀM TRÌNH HIỂN THỊ MENU*********************************************************************************
//********************************************************************************************************
void Menu_Display(byte slct) {
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(30, 0);
  display.print("====MENU====");

  display.setCursor(7, 20);
  display.print("Set Time and Date");

  display.setCursor(7, 35);
  display.print("Set Alarm");

  display.setCursor(7, 55);
  display.print("Back");

  if (slct == 1) {
    display.drawBitmap(0, 20, Select_Side, 3, 5, WHITE); 
  } else if (slct == 2) {
    display.drawBitmap(0, 35, Select_Side, 3, 5, WHITE); 
  } else if (slct == 3) {
    display.drawBitmap(0, 55, Select_Side, 3, 5, WHITE); 
  } 

  display.display();
}

//HÀM PHÁT ÂM THANH BUZZER********************************************************************************
//********************************************************************************************************
void Button_Sound(byte snd) {
  digitalWrite(Buzzer, snd);
}

//HÀM VẼ ĐỒNG HỒ ANALOG***********************************************************************************
//********************************************************************************************************
void Analog_Clock(byte h, byte m, byte s) {
  Draw_Line_Seconds(s);
  Draw_Line_Minutes(m);
  Draw_Line_Hours(h,m);
  
  for (byte i = 30; i < 33; i++) {
    display.drawPixel(i, 30, SH1106_BLACK);
    display.drawPixel(i, 31, SH1106_BLACK);
    display.drawPixel(i, 32, SH1106_BLACK);
  }
}

//HÀM VẼ KIM GIÂY ĐỒNG HỒ ANALOG**************************************************************************
//********************************************************************************************************
void Draw_Line_Seconds(int s) {
  float angle = s * 6 ;
  angle = (angle / 57.29577951); //Chuyển đổi từ độ sang radian
   
  //Độ dài kim là 16
  int xS = (clockCenterX + (sin(angle) * (16)));
  int yS = (clockCenterY - (cos(angle) * (16)));

  //Điều kiện đặc biệt ở giây thứ 15, vì tại giây đó đường thẳng không đối xứng
  if (s == 15) {
    display.drawLine(clockCenterX, clockCenterY, xS, yS + 1, WHITE);
  } else {
    display.drawLine(clockCenterX, clockCenterY, xS, yS, WHITE);
  }
  //----------------------------------------
}

//HÀM VẼ KIM PHÚT ĐỒNG HỒ ANALOG**************************************************************************
//********************************************************************************************************
void Draw_Line_Minutes(int m) {
  float angle = m * 6 ;
  angle = (angle / 57.29577951) ; //Chuyển đổi từ độ sang radian 

  //Độ dài kim là 17
  int xM = (clockCenterX + (sin(angle) * (17)));
  int yM = (clockCenterY - (cos(angle) * (17)));

  //Điều kiện đặc biệt ở phút 15, vì ở phút đó đường thẳng không đối xứng
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
  //----------------------------------------
}

//HÀM VẼ KIM GIỜ ĐỒNG HỒ ANALOG***************************************************************************
//********************************************************************************************************
void Draw_Line_Hours(int h, int m) {
  float angle = h * 30 + int((m / 12) * 6);
  angle = (angle / 57.29577951) ; //Chuyển đổi từ độ sang radian

  // Độ dài kim giờ là 12
  int xH = (clockCenterX + (sin(angle) * (12)));
  int yH = (clockCenterY - (cos(angle) * (12)));

  //Điều kiện đặc biệt ở 12h, vì tại giây đó đường thẳng không đối xứng
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
  //----------------------------------------
}

//HÀM VẼ KHUNG ĐỒNG HỒ ANALOG*****************************************************************************
//********************************************************************************************************
void Draw_Clock_Frame() {
  display.drawBitmap(0, 0, Clock_Frame, 63, 63, WHITE);
}

//HÀM CÀI ĐẶT THỜI GIAN***********************************************************************************
//********************************************************************************************************
void Set_Time_Date() {
  read_button();

  display.clearDisplay();
  
  Digital_Clock_and_Dateee(_hour24, _minute, _second, _dtw, _day, _month ,_year);
  
  display.setCursor(7, 55);
  display.print("Back");

  display.setCursor(7, 45);
  display.print("Save");

  if (btn_Down == HIGH) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Menu_Set_TimeDate++;
    if (Menu_Set_TimeDate > 7) Menu_Set_TimeDate = 1;
  }

  if (btn_Up == HIGH) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Menu_Set_TimeDate--;
    if (Menu_Set_TimeDate < 1) Menu_Set_TimeDate = 7;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 1) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    set_Hour = true;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 2) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    set_Minute = true;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 3) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    set_Day = true;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 4) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    set_Month = true;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 5) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    set_Year = true;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 6) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    rtc.adjust(DateTime(_year, _month, _day, _hour24, _minute, 0));
    Display_to_save();
  }

  if (btn_Menu_Select == HIGH && Menu_Set_TimeDate == 7) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Menu_Set_cnt = 0;
    Menu_Set = false;
  }

  if (Menu_Set_TimeDate == 1) { //Vi tri con tro
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 2) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 3) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 4) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 5) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 6) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } else if (Menu_Set_TimeDate == 7) {
    Set_Time_Date_Display(Menu_Set_TimeDate);
  } 

  display.display();

  while (set_Hour == true) {  //Vong lap cai dat gio
    read_button();
    display.clearDisplay();

    display.setCursor(70, 35);
    display.print("Set");
    
    display.setCursor(60, 45);
    display.print("the hour");

    display.setCursor(7, 55);
    display.print("Back");
  
    display.setCursor(7, 45);
    display.print("Save");
    
    Digital_Clock_and_Dateee(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    if (btn_Down == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _hour24++;
      if (_hour24 > 23) _hour24 = 0;
    }
  
    if (btn_Up == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _hour24--;
      if (_hour24 > 254) _hour24 = 23;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      set_Hour = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      delay(100);
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(38, 10, Select_Top, 5, 3, WHITE); 
    } else {
      display.drawBitmap(38, 10, Select_Top, 5, 3, BLACK); 
    }
    
    display.display();
    delay(50);
  }
 
  while (set_Minute == true) {  //Vong lap cai dat phut
    read_button();
    display.clearDisplay();

    display.setCursor(70, 35);
    display.print("Set");
    
    display.setCursor(60, 45);
    display.print("the minute");

    display.setCursor(7, 55);
    display.print("Back");
  
    display.setCursor(7, 45);
    display.print("Save");

    if (btn_Down == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _minute++;
      if (_minute > 59) _minute = 0;
    }
  
    if (btn_Up == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _minute--;
      if (_minute > 254) _minute = 59;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      set_Minute = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      delay(100);
    }

    Digital_Clock_and_Dateee(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(56, 10, Select_Top, 5, 3, WHITE);
    } else {
      display.drawBitmap(56, 10, Select_Top, 5, 3, BLACK); 
    }
    
    display.display();
    delay(50);
  }

  while (set_Day == true) { //Vong lap cai dat ngay
    read_button();
    display.clearDisplay();

    display.setCursor(70, 35);
    display.print("Set");
    
    display.setCursor(60, 45);
    display.print("the day");

    display.setCursor(7, 55);
    display.print("Back");
  
    display.setCursor(7, 45);
    display.print("Save");

    if (btn_Down == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _day++;
      if (_day > 31) _day = 1;
    }
  
    if (btn_Up == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _day--;
      if (_day < 1) _day = 31;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      set_Day = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      delay(100);
    }

    Digital_Clock_and_Dateee(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(63, 25, Select_Top, 5, 3, WHITE); 
    } else {
      display.drawBitmap(63, 25, Select_Top, 5, 3, BLACK); 
    }
    
    display.display();
    delay(100);
  }

  while (set_Month == true) { //Vong lap cai dat thang
    read_button();
    display.clearDisplay();

    display.setCursor(70, 35);
    display.print("Set");
    
    display.setCursor(60, 45);
    display.print("the month");

    display.setCursor(7, 55);
    display.print("Back");
  
    display.setCursor(7, 45);
    display.print("Save");

    if (btn_Down == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _month++;
      if (_month > 12) _month = 1;
    }
  
    if (btn_Up == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _month--;
      if (_month < 1) _month = 12;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      set_Month = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      delay(100);
    }

    Digital_Clock_and_Dateee(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(78, 25, Select_Top, 5, 3, WHITE); 
    } else {
      display.drawBitmap(78, 25, Select_Top, 5, 3, BLACK); 
    }
    
    display.display();
    delay(50);
  }

  while (set_Year == true) {  //Vong lap cai dat nam
    read_button();
    display.clearDisplay();

    display.setCursor(70, 35);
    display.print("Set");
    
    display.setCursor(60, 45);
    display.print("the year");

    display.setCursor(7, 55);
    display.print("Back");
  
    display.setCursor(7, 45);
    display.print("Save");

    if (btn_Down == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _year++;
      if (_year > 2100) _year = 2000;
    }
  
    if (btn_Up == HIGH) {
      delay(50);
      Button_Sound(0);
      delay(50);
      Button_Sound(1);
      _year--;
      if (_year < 2000) _year = 2100;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      set_Year = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      delay(100);
    }

    Digital_Clock_and_Dateee(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(110, 25, Select_Top, 5, 3, WHITE); 
    } else {
      display.drawBitmap(110, 25, Select_Top, 5, 3, BLACK); 
    }
    
    display.display();
    delay(50);
  }
}

//HÀM HIỂN THỊ CÀI ĐẶT THỜI GIAN**************************************************************************
//********************************************************************************************************
void Set_Time_Date_Display(byte slc) {
   display.setTextSize(1);
   display.setTextColor(WHITE);
    
  if (slc == 1) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the hour");
    display.drawBitmap(38, 10, Select_Top, 5, 3, WHITE); 
  } else if (slc == 2) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the minute");
    display.drawBitmap(56, 10, Select_Top, 5, 3, WHITE); 
  } else if (slc == 3) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the day");
    display.drawBitmap(63, 25, Select_Top, 5, 3, WHITE); 
  } else if (slc == 4) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the month");
    display.drawBitmap(78, 25, Select_Top, 5, 3, WHITE); 
  } else if (slc == 5) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the year");
    display.drawBitmap(110, 25, Select_Top, 5, 3, WHITE); 
  } else if (slc == 6) {
    display.setCursor(70, 35);
    display.print("Save");
    display.setCursor(60, 45);
    display.print("Settings");
    display.drawBitmap(0, 45, Select_Top, 3, 5, WHITE); 
  } else if (slc == 7) {
    display.drawBitmap(0, 55, Select_Top, 3, 5, WHITE); 
  } 
}

//HÀM CÀI ĐẶT BÁO THỨC************************************************************************************
//********************************************************************************************************
void Set_Alarm() {
  read_button();
  
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  Set_Alarm_Display();

  //NÚT NHẤN CÀI ĐẶT BÁO THỨC (GIỜ-PHÚT)
  if (btn_Down == HIGH) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Menu_Set_Alarm++;
    if (Menu_Set_Alarm > 5) Menu_Set_Alarm = 1;
  }

  if (btn_Up == HIGH) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Menu_Set_Alarm--;
    if (Menu_Set_Alarm < 1) Menu_Set_Alarm = 5;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_Alarm == 1) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Set_hour_alarm = true;
  }

  if (btn_Menu_Select == HIGH && Menu_Set_Alarm == 2) {
    Button_Sound(0);
    delay(100);
    Button_Sound(1);
    Set_minute_alarm = true;
  }

  //NÚT BẬT VÀ LƯU BÁO THỨC
  if (btn_Menu_Select == HIGH && Menu_Set_Alarm == 3) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    Display_to_save();
    Alarm_Stat = 1;
    EEPROM.update(Alarm_Stat_address, Alarm_Stat);
    delay(100);
    EEPROM.update(hour_alarm_address, hour_alarm);
    delay(100);
    EEPROM.update(minute_alarm_address, minute_alarm); 
    delay(100);

    Alarm_Stat = EEPROM.read(Alarm_Stat_address);
    delay(100);
    hour_alarm = EEPROM.read(hour_alarm_address);
    delay(100);
    minute_alarm = EEPROM.read(minute_alarm_address); 
    delay(100);
  }

  //NÚT NHẤN TẮT BÁO THỨC
  if (btn_Menu_Select == HIGH && Menu_Set_Alarm == 4) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    Display_to_save();
    
    Alarm_Stat = 0;
    hour_alarm = 0;
    minute_alarm = 0;
    EEPROM.update(Alarm_Stat_address, Alarm_Stat);
    delay(100);
    EEPROM.update(hour_alarm_address, hour_alarm);
    delay(100);
    EEPROM.update(minute_alarm_address, minute_alarm); 
    delay(100);

    Alarm_Stat = EEPROM.read(Alarm_Stat_address);
    delay(100);
    hour_alarm = EEPROM.read(hour_alarm_address);
    delay(100);
    minute_alarm = EEPROM.read(minute_alarm_address); 
    delay(100);
  }
 
  //NÚT QUAY TRỞ VỀ MENU CHÍNH
  if (btn_Menu_Select == HIGH && Menu_Set_Alarm == 5) {
    Button_Sound(0);
    delay(500);
    Button_Sound(1);
    Menu_Set_Alarm = 0;
    Menu_Set = false;
  }

  //HIỂN THỊ CON TRỎ KHI CÀI ĐẶT BÁO THỨC
  if (Menu_Set_Alarm == 1) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 2) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 3) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 4) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } else if (Menu_Set_Alarm == 5) {
    Set_Alarm_Cursor(Menu_Set_Alarm);
  } 

  while (Set_hour_alarm == true && Menu_Set_Alarm == 1) {     //Vòng lặp cài đặt giờ báo thức
    read_button();
    
    display.clearDisplay();

    if (btn_Down == HIGH) {
      _delay = 50;
      Button_Sound(0);
      delay(_delay);
      Button_Sound(1);
      hour_alarm++;
      if (hour_alarm > 23) hour_alarm = 0;
    }
  
    if (btn_Up == HIGH) {
      _delay = 50;
      Button_Sound(0);
      delay(_delay);
      Button_Sound(1);
      hour_alarm--;
      if (hour_alarm > 254) hour_alarm = 23;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      Set_hour_alarm = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      _delay = 100;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(51, 26, Select_Top, 5, 3, WHITE); 
    } else {
      display.drawBitmap(51, 26, Select_Top, 5, 3, BLACK); 
    }

    display.display();
    delay(_delay);
  }

  while (Set_minute_alarm == true && Menu_Set_Alarm == 2) {       //Vòng lặp cài đặt phút báo thức
    read_button();
    
    display.clearDisplay();

    if (btn_Down == HIGH) {
      _delay = 50;
      Button_Sound(0);
      delay(_delay);
      Button_Sound(1);
      minute_alarm++;
      if (minute_alarm > 59) minute_alarm = 0;
    }
  
    if (btn_Up == HIGH) {
      _delay = 50;
      Button_Sound(0);
      delay(_delay);
      Button_Sound(1);
      minute_alarm--;
      if (minute_alarm > 254) minute_alarm = 59;
    }

    if (btn_Menu_Select == HIGH) {
      Button_Sound(0);
      delay(500);
      Button_Sound(1);
      Set_minute_alarm = false;
    }

    if (btn_Down == LOW && btn_Up == LOW && btn_Menu_Select == LOW) {
      _delay = 100;
    }

    Set_Alarm_Display();

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(69, 26, Select_Top, 5, 3, WHITE); 
    } else {
      display.drawBitmap(69, 26, Select_Top, 5, 3, BLACK); 
    }

    display.display();
    delay(_delay);
  }
 
  
  display.display();
}

//HÀM HIỂN THỊ CÀI ĐẶT BÁO THỨC***************************************************************************
//********************************************************************************************************
void Set_Alarm_Display() {
  if (Alarm_Stat == 0) {
    display.setCursor(15, 0);
    display.print("Status : Disable");
  } else {
    display.setCursor(18, 0);
    display.print("Status : Enable");
  }

  if (hour_alarm < 10) {
    display.setCursor(48, 15);
    display.print("0");
    display.print(hour_alarm);
  } else {
    display.setCursor(48, 15);
    display.print(hour_alarm);
  }
  display.print(":");
  if (minute_alarm < 10) {
    display.print("0");
    display.print(minute_alarm);
  } else {
    display.print(minute_alarm);
  }

  display.setCursor(7, 35);
  display.print("Enable");

  display.setCursor(7, 45);
  display.print("Disable");

  display.setCursor(7, 55);
  display.print("Back");
}

//HÀM HIỂN THỊ VỊ TRÍ CON TRỎ KHI CÀI ĐẶT BÁO THỨC********************************************************
//********************************************************************************************************
void Set_Alarm_Cursor(byte slc) {
  if (slc == 1) {
    display.drawBitmap(51, 26, Select_Top, 5, 3, WHITE); 
  } else if (slc == 2) {
    display.drawBitmap(69, 26, Select_Top, 5, 3, WHITE); 
  } else if (slc == 3) {
    display.drawBitmap(0, 35, Select_Side, 3, 5, WHITE); 
  } else if (slc == 4) {
    display.drawBitmap(0, 45, Select_Side, 3, 5, WHITE); 
  } else if (slc == 5) {
    display.drawBitmap(0, 55, Select_Side, 3, 5, WHITE); 
  } 
}

//HÀM HIỂN THỊ KHI LƯU KẾT QUẢ****************************************************************************
//********************************************************************************************************
void Display_to_save() {
  display.clearDisplay(); 
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(33, 24);
  display.print("Saved");
  
  display.display();

  Button_Sound(0);
  delay(500);
  Button_Sound(1);
  delay(100);
  Button_Sound(0);
  delay(500);
  Button_Sound(1);
  delay(100);
}

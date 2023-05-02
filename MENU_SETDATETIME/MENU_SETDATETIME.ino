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
}
void loop() {
  unsigned long currentMillisGetTimeDate = millis(); //sử dụng hàm millis() để lấy thời gian hiện tại và gán giá trị cho biến currentMillisGetTimeDate
  if (currentMillisGetTimeDate - previousMillisGetTimeDate >= intervalGetTimeDate) {  //kiểm tra xem đã đến thời điểm lấy thời gian và ngày tháng hay chưa. Nếu thời gian đã trôi qua khoảng thời gian được đặt bởi hằng số intervalGetTimeDate.
    previousMillisGetTimeDate = currentMillisGetTimeDate;//cập nhật giá trị của biến previousMillisGetTimeDate bằng currentMillisGetTimeDate
    if (Menu_Stat == false) { // bnt menu_stat không nhấn tức là False. Nếu đúng, thực hiện các lệnh để lấy thời gian và ngày tháng từ hàm GetDateTime()
      GetDateTime();  // xóa màn hình OLED bằng hàm display.clearDisplay() và hiển thị thời gian và ngày tháng trên màn hình OLED bằng hàm Digital_Clock_and_Date()
      DHT11_read();
      display.clearDisplay();
      Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);
      DHT11_display();
      }
      display.display();  //hiển thị nội dung đã được cập nhật trên màn hình OLED.
    }
  read_button();  //Đọc nút nhấn
  check_button(); //Kiểm tra trạng thái nút nhấn
}

//CHƯƠNG TRÌNH HIỂN THỊ THÔNG TIN CỦA NHIỆT ĐỘ VÀ ĐỘ ẨM***************************************************
//********************************************************************************************************
void DHT11_display(){
  if (isnan(doam) || isnan(doC)) 
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 40);
    display.println("Error to read DHT11!");
    return;
  }
  display.setCursor(0, 35);
  display.print("Temp:");
  display.print(doC); 
  display.print("C");
  display.setCursor(0, 55);
  display.print("Hum:");
  display.print(doam);
  display.print("%");  
  if(doC > 35)
  {
    display.setCursor(0, 45);
    display.print("High temp!!!");
    Button_Sound(1);       
    delay(100);
    Button_Sound(0);
  }
}

//CHƯƠNG TRÌNH ĐỌC NÚT NHẤN*******************************************************************************
//********************************************************************************************************
void read_button() {
  btn_Menu_Select = digitalRead(4);
  btn_Up = digitalRead(5);
  btn_Down = digitalRead(6);
}

//CHƯƠNG TRÌNH KIỂM TRA TRẠNG THÁI NÚT NHẤN***************************************************************
//********************************************************************************************************
void check_button() {
  if (btn_Menu_Select == LOW && Menu_Stat == false) { //Nếu nút btn_Menu_Select được nhấn và trạng thái Menu_Stat là false.
    Button_Sound(1);  //Phát ra âm thanh để cho biết nút đã được nhấn.
    delay(500);      
    Button_Sound(0);  //Tắt âm thanh
    Menu_Cnt = 1;     //Thiết lập biến Menu_Cnt lên 1.
    Menu_Stat = true; //Thiết lập lại trạng thái Menu_Stat là true.
  } 
  while (Menu_Stat == true) { //Lặp lại cho đến khi Menu_Stat là false.
    read_button();            //Đọc trạng thái các nút nhấn. 
    if (btn_Down == LOW) {    //nút btn_Down được nhấn.
      Button_Sound(1);        //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(100);
      Button_Sound(0);        //Tắt âm thanh.
      Menu_Cnt++;             //Tăng giá trị của Menu_Cnt lên 1.
      if (Menu_Cnt > 3) Menu_Cnt = 1; //Nếu giá trị của Menu_Cnt vượt quá 3 thì thiết lập nó về 1.
    }
  
    if (btn_Up == LOW) {      //nút btn_Up được nhấn.
      Button_Sound(1);        //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(100);
      Button_Sound(0);        //Tắt âm thanh.
      Menu_Cnt--;             //Giảm giá trị của Menu_Cnt lên 1.
      if (Menu_Cnt < 1) Menu_Cnt = 3; //Nếu giá trị của Menu_Cnt vượt quá 1 thì thiết lập nó về 3.
    }

    if (btn_Menu_Select == LOW && Menu_Cnt == 1) { //Nếu bnt_Menu_Select được nhấn và Menu_Cnt ==1
      Button_Sound(1);  //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(500);
      Button_Sound(0);  //Tắt âm thanh.
      Menu_Set = true;  //Thiết lập lại trạng thái Menu_Set là true.
      Menu_Set_cnt = 1; //Thiết lập lại trạng thái Menu_Set_Cnt là 1.  
    }
  
    if (btn_Menu_Select == LOW && Menu_Cnt == 2) { //Nếu bnt_Menu_Select được nhấn và Menu_Cnt ==2
      Button_Sound(1);  //Phát ra âm thanh để cho biết nút đã được nhấn.
      delay(500);
      Button_Sound(0);  //Tắt âm thanh.
      Menu_Set = true;  //Thiết lập lại trạng thái Menu_Set là true.
      Menu_Set_cnt = 2; //Thiết lập lại trạng thái Menu_Set_Cnt là 2. 
      delay(100);
    }

    if (btn_Menu_Select == LOW && Menu_Cnt == 3) {  //Nếu bnt_Menu_Select được nhấn và Menu_Cnt ==3
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
      //SET_ALARM
      delay(50);
    }
    delay(50);
  }
}

//CHƯƠNG TRÌNH HIỂN THỊ THÔNG TIN VỀ THƠI GIAN************************************************************
//********************************************************************************************************
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

void Digital_Clock_and_Date(byte h, byte m, byte s, byte dtw, byte dy, byte mt, int yr) {
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

//CHƯƠNG TRÌNH HIỂN THỊ MENU******************************************************************************
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
void Button_Sound(byte snd) {
  digitalWrite(Buzzer, snd);
}


void Set_Time_Date() {
  read_button();

  display.clearDisplay();
  
  Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);
  
  display.setCursor(7, 55);
  display.print("Back");

  display.setCursor(7, 45);
  display.print("Save");

  //----------------------------------------Condition of Buttons for selecting Time Settings
  if (btn_Down == LOW) {
    Button_Sound(1);
    delay(100);
    Button_Sound(0);
    Menu_Set_TimeDate++;
    if (Menu_Set_TimeDate > 7) Menu_Set_TimeDate = 1;
  }

  if (btn_Up == LOW) {
    Button_Sound(1);
    delay(100);
    Button_Sound(0);
    Menu_Set_TimeDate--;
    if (Menu_Set_TimeDate < 1) Menu_Set_TimeDate = 7;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 1) {
    Button_Sound(1);
    delay(500);
    Button_Sound(0);
    set_Hour = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 2) {
    Button_Sound(1);
    delay(500);
    Button_Sound(0);
    set_Minute = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 3) {
    Button_Sound(1);
    delay(500);
    Button_Sound(0);
    set_Day = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 4) {
    Button_Sound(1);
    delay(500);
    Button_Sound(0);
    set_Month = true;
  }

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 5) {
    Button_Sound(1);
    delay(500);
    Button_Sound(0);
    set_Year = true;
  }

  //~~~~~~~~~~~~~~~~~~~Button Condition for saving Time and Date settings
  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 6) {
    Button_Sound(1);
    delay(500);
    Button_Sound(0);
    rtc.adjust(DateTime(_year, _month, _day, _hour24, _minute, 0));
    Display_to_save();
  }
  //~~~~~~~~~~~~~~~~~~~

  if (btn_Menu_Select == LOW && Menu_Set_TimeDate == 7) {
    Button_Sound(1);
    delay(100);
    Button_Sound(0);
    Menu_Set_cnt = 0;
    Menu_Set = false;
  }
  //---------------------------------------- 

  //----------------------------------------Conditions for the cursor position
  if (Menu_Set_TimeDate == 1) {
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
  //---------------------------------------- 

  display.display();

  //----------------------------------------Looping to set Hour
  while (set_Hour == true) {
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
    
    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    if (btn_Down == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _hour24++;
      if (_hour24 > 23) _hour24 = 0;
    }
  
    if (btn_Up == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _hour24--;
      if (_hour24 > 254) _hour24 = 23;
    }

    if (btn_Menu_Select == LOW) {
      Button_Sound(1);
      delay(500);
      Button_Sound(0);
      set_Hour = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH) {
      delay(100);
    }

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(38, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    } else {
      display.drawBitmap(38, 10, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    
    display.display();
    delay(50);
  }
  //---------------------------------------- 

  //----------------------------------------Looping to set Minute
  while (set_Minute == true) {
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

    if (btn_Down == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _minute++;
      if (_minute > 59) _minute = 0;
    }
  
    if (btn_Up == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _minute--;
      if (_minute > 254) _minute = 59;
    }

    if (btn_Menu_Select == LOW) {
      Button_Sound(1);
      delay(500);
      Button_Sound(0);
      set_Minute = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH) {
      delay(100);
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(56, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    } else {
      display.drawBitmap(56, 10, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    
    display.display();
    delay(50);
  }
  //---------------------------------------- 

  //----------------------------------------Looping to set Day / Date
  while (set_Day == true) {
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

    if (btn_Down == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _day++;
      if (_day > 31) _day = 1;
    }
  
    if (btn_Up == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _day--;
      if (_day < 1) _day = 31;
    }

    if (btn_Menu_Select == LOW) {
      Button_Sound(1);
      delay(500);
      Button_Sound(0);
      set_Day = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH) {
      delay(100);
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(63, 25, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    } else {
      display.drawBitmap(63, 25, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    
    display.display();
    delay(100);
  }
  //---------------------------------------- 

  //----------------------------------------Looping to set Month
  while (set_Month == true) {
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

    if (btn_Down == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _month++;
      if (_month > 12) _month = 1;
    }
  
    if (btn_Up == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _month--;
      if (_month < 1) _month = 12;
    }

    if (btn_Menu_Select == LOW) {
      Button_Sound(1);
      delay(500);
      Button_Sound(0);
      set_Month = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH) {
      delay(100);
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(78, 25, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    } else {
      display.drawBitmap(78, 25, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    
    display.display();
    delay(50);
  }
  //---------------------------------------- 

  //----------------------------------------Looping to set Year
  while (set_Year == true) {
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

    if (btn_Down == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _year++;
      if (_year > 2100) _year = 2000;
    }
  
    if (btn_Up == LOW) {
      delay(50);
      Button_Sound(1);
      delay(50);
      Button_Sound(0);
      _year--;
      if (_year < 2000) _year = 2100;
    }

    if (btn_Menu_Select == LOW) {
      Button_Sound(1);
      delay(500);
      Button_Sound(0);
      set_Year = false;
    }

    if (btn_Down == HIGH && btn_Up == HIGH && btn_Menu_Select == HIGH) {
      delay(100);
    }

    Digital_Clock_and_Date(_hour24, _minute, _second, _dtw, _day, _month ,_year);

    blink_cursor = !blink_cursor;
    if (blink_cursor == true) {
      display.drawBitmap(110, 25, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    } else {
      display.drawBitmap(110, 25, Select_Top, 5, 3, BLACK); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    }
    
    display.display();
    delay(50);
  }
  //---------------------------------------- 
}
//________________________________________________________________________

//________________________________________________________________________Subroutines for displaying a cursor to select settings
void Set_Time_Date_Display(byte slc) {
   display.setTextSize(1);
   display.setTextColor(WHITE);
    
  if (slc == 1) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the hour");
    display.drawBitmap(38, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else if (slc == 2) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the minute");
    display.drawBitmap(56, 10, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else if (slc == 3) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the day");
    display.drawBitmap(63, 25, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else if (slc == 4) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the month");
    display.drawBitmap(78, 25, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else if (slc == 5) {
    display.setCursor(70, 35);
    display.print("Set");
    display.setCursor(60, 45);
    display.print("the year");
    display.drawBitmap(110, 25, Select_Top, 5, 3, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else if (slc == 6) {
    display.setCursor(70, 35);
    display.print("Save");
    display.setCursor(60, 45);
    display.print("Settings");
    display.drawBitmap(0, 45, Select_Top, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else if (slc == 7) {
    display.drawBitmap(0, 55, Select_Top, 3, 5, WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } 
}
//
void Display_to_save() {
  display.clearDisplay(); 
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(33, 24);
  display.print("Saved");
  
  display.display();

  Button_Sound(1);
  delay(500);
  Button_Sound(0);
  delay(100);
  Button_Sound(1);
  delay(500);
  Button_Sound(0);
  delay(100);
}
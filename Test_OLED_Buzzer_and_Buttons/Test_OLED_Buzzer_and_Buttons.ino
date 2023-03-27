#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define SCREEN_WIDTH 128 //định nghĩa chiều rộng của màn hình OLED.
#define SCREEN_HEIGHT 64 //định nghĩa chiều cao của màn hình OLED.

#define OLED_RESET     -1  // định nghĩa chân reset của màn hình OLED.
Adafruit_SH1106 display(OLED_RESET);//khởi tạo đối tượng display cho màn hình OLED với các thông số chiều rộng, chiều cao, thiết bị I2C và chân reset.

byte btn_Menu_Select, btn_Up, btn_Down; //khai báo các biến để lưu trữ trạng thái của các nút bấm.

byte Buzzer = 7; // định nghĩa chân cho còi báo

bool Button_1;   //khai báo các biến để lưu trữ trạng thái của các nút bấm.
bool Button_2;   
bool Button_3;   

void setup() {   
  Serial.begin(9600); //khởi tạo cổng giao tiếp Serial với tốc độ baud 9600.

  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP); 
  pinMode(6, INPUT_PULLUP); 

  pinMode(Buzzer, OUTPUT);  

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
 
  delay(1000);
}

void loop() {
  read_button();  //Gọi đến hàm read_button() để đọc trạng thái các nút bấm
  display.clearDisplay(); //Xóa nội dung trên màn hình OLED.
  display.setTextSize(1); //Thiết lập kích thước chữ trên màn hình OLED là 1.
  display.setTextColor(WHITE);  //Thiết lập màu chữ trên màn hình OLED là trắng.
  display.setCursor(0,0); // Thiết lập vị trí con trỏ đến vị trí (0,0) trên màn hình OLED.
  display.print("Button 1 : ");// In ra chuỗi "Button 1 : " trên màn hình OLED.
  Button_1 = btn_Menu_Select; //Gán giá trị của biến btn_Menu_Select vào biến Button_1.
  if (Button_1 == LOW) {  // Kiểm tra xem Button_1 có được nhấn hay không.
    display.print("Pressed"); // In ra chuỗi "Pressed" trên màn hình OLED nếu Button_1 được nhấn.
    Button_Sound(1);  //Gọi đến hàm Button_Sound() để phát ra âm thanh khi Button_1 được nhấn.
  } else {
    display.print("UnPress"); //In ra chuỗi "UnPress" trên màn hình OLED nếu Button_1 không được nhấn.
  }
  display.setCursor(0,10);
  display.print("Button 2 : ");
  Button_2 = btn_Up;
  if (Button_2 == LOW) {
    display.print("Pressed");
    Button_Sound(1);
  } else {
    display.print("UnPress");
  }

  display.setCursor(0,20); 
  display.print("Button 3 : ");
  Button_3 = btn_Down;
  if (Button_3 == LOW) {
    display.print("Pressed");
    Button_Sound(1);
  } else {
    display.print("UnPress");
  }
  if (Button_1 == HIGH && Button_2 == HIGH && Button_3 == HIGH) { //Kiểm tra xem các nút bấm có đều không được nhấn hay không.
    Button_Sound(0);  //Gọi đến hàm Button_Sound() để tắt âm thanh nếu các nút bấm không được nhấn.
  }
  display.setCursor(0,55); //Thiết lập vị trí con trỏ đến vị trí (0,55) trên màn hình OLED.
  display.print("Buzzer : ");
  int Buzzer_Stat = digitalRead(Buzzer);  // Đọc trạng thái của chân kết nối với loa phát ra âm thanh và gán giá trị vào biến Buzzer_Stat.
  if (Buzzer_Stat == 1) { //Kiểm tra xem loa phát ra âm thanh có đang hoạt động hay không.
    display.print("On");  //In ra chuỗi "On" trên màn hình OLED nếu Buzzer hoạt động.
  } else {
    display.print("Off"); //In ra chuỗi "Off" trên màn hình OLED nếu Buzzer không hoạt động.
  }
  display.display();
  delay(100);
}
void read_button() {  //đọc trạng thái của ba nút bấm Menu Select, Up, Down bằng cách sử dụng hàm digitalRead() và lưu kết quả vào ba biến btn_Menu_Select, btn_Up, và btn_Down.
  btn_Menu_Select = digitalRead(4);
  btn_Up = digitalRead(5);
  btn_Down = digitalRead(6);
}
void Button_Sound(byte snd) { //điều khiển đầu ra âm thanh của buzzer bằng cách sử dụng hàm digitalWrite(). Tham số snd cho biết trạng thái của buzzer: 1 để bật và 0 để tắt.
  digitalWrite(Buzzer, snd);
}
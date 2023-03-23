#include <Wire.h> //Thư viện dùng để giao tiếp với các thiết bị I2C, như màn hình OLED và RTC.
#include <Adafruit_GFX.h> //Thư viện dùng để vẽ các đối tượng đồ họa trên màn hình OLED.
#include <Adafruit_SH1106.h> //Thư viện dùng để điều khiển màn hình OLED loại SH1106.
#include "RTClib.h" //Thư viện dùng để điều khiển module RTC DS1307.
#include <DHT.h>  //Thư viện dùng để đọc dữ liệu từ cảm biến nhiệt độ độ ẩm DHT11.

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

RTC_DS1307 rtc; //Khai báo đối tượng RTC_DS1307 có tên là rtc để sử dụng cho module RTC.

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SH1106 display(OLED_RESET);////Khai báo đối tượng Adafruit_SH1106 có tên là display để điều khiển màn hình OLED. Tham số OLED_RESET là chân reset của màn hình OLED, trong trường hợp này là -1, tức không sử dụng chức năng reset.

#define DHTPIN 2     // Chân kết nối DHT11 trên board
#define DHTTYPE DHT11   // Loại cảm biến là DHT11
DHT dht(DHTPIN, DHTTYPE); //Khai báo đối tượng DHT có tên là dht để sử dụng cho cảm biến DHT11. Tham số đầu tiên là chân kết nối cảm biến, tham số thứ hai là loại cảm biến.

const float NHIET_DO_CAO = 35.0; //Khai bao 1 bien nhiet do cao = 35 do C

byte Buzzer = 7; // định nghĩa chân cho còi báo

char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(115200); //Giao tiếp với tốc độ baud 115200
  pinMode(Buzzer, OUTPUT);
    if (! rtc.begin()) {  //kiểm tra xem thiết bị RTC (Real-Time Clock - Đồng hồ thời gian thực) có sẵn và hoạt động hay không.
    Serial.println("Couldn't find RTC");  //Nếu không kết nối được, chương trình sẽ hiển thị thông báo lỗi "Couldn't find RTC" và dừng lại vô thời hạn.
    while (1);
  }

  if (! rtc.isrunning()) {  //kiểm tra xem RTC đang chạy hay không
    Serial.println("RTC is NOT running!");abort();  //Nếu RTC không chạy, chương trình sẽ hiển thị thông báo lỗi "RTC is NOT running!" và dừng lại ngay lập tức.
  }
  dht.begin(); //để kết nối với cảm biến đo nhiệt độ và độ ẩm DHT để đọc dữ liệu từ cảm biến.

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);//khởi động màn hình OLED. Tham số đầu tiên SH1106_SWITCHCAPVCC cho biết loại màn hình và cách cung cấp điện áp cho nó. Tham số thứ hai 0x3C cho biết địa chỉ I2C của màn hình.
  delay(1000);
  display.clearDisplay(); //Xóa bộ nhớ đệm của màn hình OLED.
}

void loop() {
  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

   display.setCursor(35, 0);
  if (now.hour() < 10)
    display.print('0');
  display.print(now.hour());
  display.print(':');
  if (now.minute() < 10)
    display.print('0');
  display.print(now.minute());
  display.print(':');
  if (now.second() < 10)
    display.print('0');
  display.println(now.second());

  display.setCursor(0, 10);
  display.println(days[now.dayOfTheWeek()]);
  display.setCursor(50, 10);
  display.print(',');
  display.setCursor(60, 10);
  display.print(now.day());
  display.print('/');
  display.print(now.month());
  display.print('/');
  display.print(now.year());
  
  float temperature = dht.readTemperature();  // Đọc giá trị nhiệt độ từ DHT11
  float humidity = dht.readHumidity();  // Đọc giá trị độ ẩm từ DHT11
  
  display.setCursor(0, 30);
  display.print("Temp: ");
  display.print(temperature);
  display.print("C");

  display.setCursor(0, 40);
  display.print("Hum: ");
  display.print(humidity);
  display.print(" %");

  if (temperature > NHIET_DO_CAO) {
    Button_Sound(1); // Bật buzzer
    delay(10000); // Giữ buzzer kêu trong 10 giây
    Button_Sound(0); // Tắt buzzer
  }
  display.display();
}
void Button_Sound(byte snd) { ////điều khiển đầu ra âm thanh của buzzer bằng cách sử dụng hàm digitalWrite(). Tham số snd cho biết trạng thái của buzzer: 1 để bật và 0 để tắt.
  digitalWrite(Buzzer, snd);
}

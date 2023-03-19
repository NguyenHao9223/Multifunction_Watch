#include <DHT.h>

const int DHTPIN = 3;//Chân Out của cảm biến nối chân số 5 Arduino

const int DHTTYPE = DHT11;   // Khai báo kiểu cảm biến là DHT11

const float NHIET_DO_CAO = 35.0;
const float DO_AM_CAO = 70.0;

DHT dht(DHTPIN, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến

byte Buzzer = 7; // định nghĩa chân cho còi báo

void setup() 
{
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT); 

  dht.begin(); //Khởi động cảm biến
}

void loop() 
{
  float doam = dht.readHumidity(); //Đọc độ ẩm

  float doC = dht.readTemperature(); //Đọc nhiệt độ C

  float doF = dht.readTemperature(true); //Đọc nhiệt độ F

  // Kiểm tra cảm biến có hoạt động hay không
  if (isnan(doam) || isnan(doC) || isnan(doF)) 
  {
    Serial.println("Không có giá trị trả về từ cảm biến DHT");
    return;
  }

  Serial.print("Độ ẩm: ");
  Serial.print(doam);
  
  Serial.print("%  Nhiệt độ: ");
  Serial.print(doC);
  
  Serial.print("°C | ");
  Serial.print(doF);
  Serial.println("°F");

// Kiểm tra nhiệt độ và độ ẩm vượt ngưỡng
  if (doC > NHIET_DO_CAO) {
    Serial.println("Cảnh báo: Nhiệt độ cao!");
    Button_Sound(1); // Bật buzzer
    delay(10000); // Giữ buzzer kêu trong 10 giây
    Button_Sound(0); // Tắt buzzer
  }

  if (doam > DO_AM_CAO) {
    Serial.println("Cảnh báo: Độ ẩm cao!");
    Button_Sound(1); // Bật buzzer
    delay(10000); // Giữ buzzer kêu trong 10 giây
    Button_Sound(0); // Tắt buzzer
  }

  delay(1000);
}
void Button_Sound(byte snd) { //điều khiển đầu ra âm thanh của buzzer bằng cách sử dụng hàm digitalWrite(). Tham số snd cho biết trạng thái của buzzer: 1 để bật và 0 để tắt.
  digitalWrite(Buzzer, snd);
}
#include <DHT.h>

const int DHTPIN = 3;//Chân Out của cảm biến nối chân số 5 Arduino
const int DHTTYPE = DHT11;   // Khai báo kiểu cảm biến là DHT11
DHT dht(DHTPIN, DHTTYPE); //Khai báo thư viện chân cảm biến và kiểu cảm biến

void setup() 
{
  Serial.begin(115200); 
  dht.begin(); //Khởi động cảm biến
}
void loop() 
{
 Read_DHT11();
}
void Read_DHT11(){
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
  delay(1000);
}

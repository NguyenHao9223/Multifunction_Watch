#include "DHT11.h"

const int DHTPIN = 3;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
float doam; // Khai báo biến toàn cục doam
float doC; // Khai báo biến toàn cục doC
void DHT11_setup()
{
  dht.begin();
}
void DHT11_read()
{
  doam = dht.readHumidity();
  doC = dht.readTemperature();
}

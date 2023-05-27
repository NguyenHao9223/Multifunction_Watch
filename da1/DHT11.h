#ifndef DHT11_H
#define DHT11_H

#include <DHT.h>

extern const int DHTPIN;
extern const int DHTTYPE;
extern DHT dht;

extern float doam; // Khai báo biến toàn cục doam
extern float doC; // Khai báo biến toàn cục doC
void DHT11_setup();
void DHT11_read();

#endif

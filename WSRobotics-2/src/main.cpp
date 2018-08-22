#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "satan";
String sub1;
int payload[3];

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}


void loop() {
  if (radio.available()) {
    //int count = 1000;
    radio.read(&payload, sizeof(payload));
    Serial.println(String(payload[0]) + " " + String(payload[1]) + " " + String(payload[2]));
    ////text.subString()
    Wire.beginTransmission(666); // transmit to device #8
    Wire.write(payload[0]);        // sends five bytes
    Wire.write(payload[1]);              // sends one byte
    Wire.write(payload[2]);              // sends one byte
    Wire.endTransmission();    // stop transmitting

  }
}

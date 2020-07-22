//-----------------------INCLUDES------------------------------------------
#include <Servo.h>
#include "FirebaseESP8266.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//-------------------------------------------------------------------------

#define HARDWARE_NUM 12345

//define the pins used in the ESP8266
#define PinT           4
#define PinPresence1   13
#define PinPresence2   12
#define Tap            16
#define TapRight       5
#define TapLeft        9
#define PinE           10

//If dont want to debug, comment
#define DEBUG
//If dont want to debug with TCP, comment
#define TCP
//If dont want to debug with serial port (arduino), comment
#define TERMINAL

#ifdef DEBUG
  #ifdef TCP
    #define TCPDebug(x) printTCP(x)
  #else
    #define TCPDebug(x)
  #endif
  #ifdef TERMINAL
    #define SerialDebug(x) Serial.println(x)
  #else
    #define SerialDebug(x)
  #endif
#else
    #define TCPDebug(x)
    #define SerialDebug(x)
#endif


//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;
void printResult(FirebaseData &data);


void setup() {
  Serial.begin(9600);

  //firebase connection, wifi connection and TCP connection (for debugging)
  connectWifi();
  fire_base();
  setupTCP();

  //setups for all motors
  setupTEMP(PinT);
  setupPresence(PinPresence1, PinPresence2);
  setupTap(Tap, TapRight, TapLeft);
  setupButtonEmerg(PinE);

}

void loop() {

  brain();
  calculaTEMP();
  motorFallen();
  motorTap();
  isEmergency();
  loopTCP();

}

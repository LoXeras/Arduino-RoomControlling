/*
 Name:		RoomControlling.ino
 Created:	19.11.2018 17:25:05
 Author:	Dario Duff aka LoXeras
*/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <stdlib.h>
#include <OLED.h>

#define HYT_ADDR 0x28       // I2C address of the HYT 939

const char* wlan_ssid = "P20";				//WLAN SSID
const char* wlan_password = "ZorK0717";									         //Password

//MYSQL Host
const char* host = "loxeras.com";


OLED display(D1, D2);


void setup() {
  Serial.begin(9600);

  delay(100);

  Wire.begin(D1, D2);            // Join I2c Bus as master

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  // Initialize display
 display.begin();




 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(wlan_ssid);

 WiFi.begin(wlan_ssid, wlan_password);

 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }

 Serial.println("");
 Serial.println("WiFi connected");
 display.print("WiFi connected");
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 Serial.print("Netmask: ");
 Serial.println(WiFi.subnetMask());
 Serial.print("Gateway: ");
 Serial.println(WiFi.gatewayIP());




}

void loop() {
  // put your main code here, to run repeatedly:
}

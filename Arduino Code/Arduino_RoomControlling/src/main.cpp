/*
 Name:		RoomControlling.ino
 Created:	19.11.2018 17:25:05
 Author:	Dario Duff aka LoXeras
*/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <stdlib.h>

#define HYT_ADDR 0x28     // I2C address of the HYT 939

const char* wlan_ssid = "Virus_Distribution_Center";				//WLAN SSID
const char* wlan_password = "-";									//Password

//MYSQL Host
const char* host = "loxeras.com";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.printf("HELLO\n" );
}

void loop() {
  // put your main code here, to run repeatedly:
}

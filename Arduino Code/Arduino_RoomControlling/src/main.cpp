/*
 Name:		RoomControlling.ino
 Created:	19.11.2018 17:25:05
 Author:	Dario Duff aka LoXeras
*/


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <stdlib.h>

#define HYT_ADDR 0x28       // I2C address of the HYT 939

const char* wlan_ssid = "P20";			             //WLAN SSID
const char* wlan_password = "RoomControlling";	 //Password


//MYSQL Host
const char* host = "loxeras.com";
const int httpPort = 80;

double temperature, humidity; //Define As Global

void writeDatabase(double humidity, double temperature){
    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // create URI for request
    // loxeras.com
    // /api/insert.php


    String trash = "&trash=1"; //Trash for data error correction

    String room = "99"; //RoomID

    String url = "/api/insert.php?r="+room+"&h="+humidity+"&t="+temperature+trash;
  //  url.concat(buffer);
    Serial.print("Requesting URL: ");
    Serial.println(url);

    // send request to server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(500);


    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    Serial.println();
    Serial.println("closing connection");

}

void readData(void){ //Reads data from HYT939
  Wire.beginTransmission(HYT_ADDR);   // Begin transmission with the HYT module
  Wire.requestFrom(HYT_ADDR, 4);      // Request 4 bytes

  // Read the bytes if they are available
  //Byte 1 and 2 = Humidity, 3 and 4 = temperaturee
  if (Wire.available() == 4) {
    int b1 = Wire.read();
    int b2 = Wire.read();
    int b3 = Wire.read();
    int b4 = Wire.read();

    Wire.endTransmission();           // End transmission, release I2C bus

    // combine the two humidity bytes
    //calculate humidity
    int rawHumidity = b1 << 8 | b2;
    // compound bitwise to get 14 bit measurement, first two bits  are status/stall bit
    rawHumidity =  (rawHumidity &= 0x3FFF);
    humidity = 100.0 / pow(2, 14) * rawHumidity; //calculate humidity


    // combine temperature bytes and calculate temperature
    b4 = (b4 >> 2); // Mask away 2 least significant bits see HYT 221 doc
    int rawTemperature = b3 << 6 | b4;
    temperature = 165.0 / pow(2, 14) * rawTemperature - 40;  //Calculate temperature in °C

  //  Serial.print("Humidity: ");
  //  Serial.print(humidity);
  //  Serial.print("% - Temperature: ");
  //  Serial.println(temperature);
  }
  else {
    Serial.println("Not enough bytes available.");
  }

}

void setup() {
  Serial.begin(9600);

  delay(100);

  Wire.begin(D1, D2);            // Join I2c Bus as master

  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

//Print Serial Debug
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(wlan_ssid);

 WiFi.begin(wlan_ssid, wlan_password);  //Open WIFI

//Wait for connection
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }

//Connections Debug Log
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 Serial.print("Netmask: ");
 Serial.println(WiFi.subnetMask());
 Serial.print("Gateway: ");
 Serial.println(WiFi.gatewayIP());


}

void loop() {
  readData();
// Serial Debug Log
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% - Temperature: ");
  Serial.println(temperature);

  writeDatabase(humidity,temperature); //Write to Database

  //Sends the NODEMCU to deepsleep mode for 60 second, it will automatically reset and reconnect and send the Data
  Serial.println("Going into deep sleep for 60 seconds");
  ESP.deepSleep(60e6); // 60e6 are 60'000'000 microseconds

}

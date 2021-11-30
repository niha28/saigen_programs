#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "DFRobotDFPlayerMini.h"

#define PIN 26 //INが接続されているピンを指定
#define NUMPIXELS 1 //LEDの数を指定
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); //800kHzでNeoPixelを駆動

HardwareSerial myHardwareSerial(1); // use HardwareSerial UART1
DFRobotDFPlayerMini myDFPlayer;

void onReceive(const uint8_t* mac_addr, const uint8_t* situ, int situ_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
      mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println();
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", situ_len);
  for (int i = 0; i < situ_len; i++) {
    Serial.print(situ[i]);
    Serial.print(" ");
    }
  if (situ[0] == 1) {
    if(situ[1]==1){
      for(int c=0; c<NUMPIXELS; c++) { //LEDを１つずつ制御
        pixels.setPixelColor(c, pixels.Color(0,255,0)); //LEDの色を設定
        pixels.show();   //LEDに色を反映
      }
      myDFPlayer.play(1);
    }
    if(situ[1]==2){
      for(int c=0; c<NUMPIXELS; c++) { //LEDを１つずつ制御
        pixels.setPixelColor(c, pixels.Color(127,127,0)); //LEDの色を設定
        pixels.show();   //LEDに色を反映
      }
      myDFPlayer.play(2);
    }
    if(situ[1]==3){
      for(int c=0; c<NUMPIXELS; c++) { //LEDを１つずつ制御
        pixels.setPixelColor(c, pixels.Color(0,0,255)); //LEDの色を設定
        pixels.show();   //LEDに色を反映
      }
      myDFPlayer.play(3);
    }
  }
  if(situ[0]==3){
      for(int c=0; c<NUMPIXELS; c++) { //LEDを１つずつ制御
        pixels.setPixelColor(c, pixels.Color(0,0,0)); //LEDの色を設定
        pixels.show();   //LEDに色を反映
      }
      myDFPlayer.play(4);
  }
}


void setup() {
  myHardwareSerial.begin(9600, SERIAL_8N1, 14, 12); // RX, TX
  Serial.begin(115200);
  Serial.println(WiFi.macAddress()); // このアドレスを送信側へ登録します
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESP-Now Init Success");
  }
  esp_now_register_recv_cb(onReceive);
  pixels.begin(); //NeoPixelを開始
  if (!myDFPlayer.begin(myHardwareSerial)) {  //Use HardwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
  for(int c=0; c<NUMPIXELS; c++) {
    pixels.setPixelColor(c, pixels.Color(0,0,0)); //LEDの色を設定
    pixels.show();   //LEDに色を反映
  }
}

void loop() {
}
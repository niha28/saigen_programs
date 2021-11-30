#include <Adafruit_NeoPixel.h>
#define PIN 26 //INが接続されているピンを指定
#define NUMPIXELS 1 //LEDの数を指定
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); //800kHzでNeoPixelを駆動

void setup() {
  pixels.begin(); //NeoPixelを開始
}

void loop() {
  pixels.clear(); //NeoPixelの出力をリセット
  for(int i=0; i<NUMPIXELS; i++) { //LEDを１つずつ制御
  pixels.setPixelColor(i, pixels.Color(255,0,0)); //LEDの色を設定
  pixels.show();   //LEDに色を反映
  delay(500); //500ms待機
  }
}


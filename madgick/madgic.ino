#include "M5StickC.h"
#include <math.h>
#include <MadgwickAHRS.h>
Madgwick MadgwickFilter;

#define PI 3.141592653589793
// LEDの設定
#define LED_PIN 10
#define LED_ON LOW
#define LED_OFF HIGH
// 不動時のジャイロの誤差
#define e_gx 12
#define e_gy -5
#define e_gz -6
// 値取得時の誤差範囲
#define d_angle 15
// 値取得回数
#define getn 4

float ax, ay, az;//加速度　
float gx, gy, gz;//角速度　
float ROLL, PITCH, YAW;//フィルタ値
float axs[getn],azs[getn],bector[getn];//保存先
// ボタンの判定，向きの保存，カウント，向き，i
int btn, muki[getn], cnt, pls, i;

void setup()
{
  M5.begin();
  Serial.begin(9600);
  M5.MPU6886.Init();//I2Cデバイスの初期化
  delay(300);
  MadgwickFilter.begin(100);//フィルタのサンプリングを100Hzで
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  pinMode(LED_PIN, OUTPUT);//LEDのピン設定
  digitalWrite(LED_PIN, LED_OFF);
  btn = 0;
  cnt = 0;
  pls = 0;
}

void loop()
{
  M5.update();
  digitalWrite(LED_PIN, LED_OFF);
  M5.MPU6886.getGyroData(&gx,&gy,&gz);//gyroの取得
  M5.MPU6886.getAccelData(&ax,&ay,&az);//accの取得
  MadgwickFilter.updateIMU(gx+e_gx , gy+e_gy, gz+e_gz, ax, ay, az);//madgwickフィルタの使用
  //フィルタから値の取得
  ROLL = MadgwickFilter.getRoll();
  PITCH = MadgwickFilter.getPitch();
  YAW  = MadgwickFilter.getYaw();

  M5.Lcd.setCursor(0, 70);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ", ROLL, PITCH, YAW);

  // 地面と並行，直角のときを判定
  if(PITCH>-d_angle && PITCH<d_angle){
    pls = 1;
  }
  if(PITCH>90-d_angle && PITCH<90){
    pls = 2;
  }
  if(PITCH>-90 && PITCH<-90+d_angle){
    pls = 3;
  }
  // ボタンを押すかつ，体と垂直なら開始
  if (M5.BtnA.wasReleased() && btn==0 && ROLL>-d_angle && ROLL<d_angle) {
    btn = 1;
    axs[cnt] = ax;
    azs[cnt] = az;
    muki[cnt] = pls;
    cnt = 1;
    M5.Lcd.setCursor(40, 0);
    M5.Lcd.printf("start");
  }
  if(cnt >= 1 && btn == 1){
  // 前回の取得した向きと違うなら値保存
  if(muki[cnt-1]!=pls){
    muki[cnt] = pls;
    axs[cnt] = ax;
    azs[cnt] = az;
    bector[cnt] = atan2(axs[cnt],azs[cnt]) * 180.0 / PI;
    M5.Lcd.setCursor(0, 15*cnt);
    M5.Lcd.printf("%d  %.2f %.2f  %d  %.2f ",cnt,axs[cnt],azs[cnt],muki[cnt],bector[cnt]);
    cnt = cnt + 1;
    digitalWrite(LED_PIN, LED_ON);
  }
  // 取得回数になったら初期化，出力
  if(cnt==getn){
    btn = 0;
    cnt = 0;
    pls = 0;
    for(i=0; i<getn; i++){
      Serial.print(i);
      Serial.print(axs[i]);
      Serial.print(azs[i]);
      Serial.print(muki[i]);
      Serial.print(bector[i]);
    }
    M5.Lcd.setCursor(40, 0);
    M5.Lcd.printf("  end  ");
  }
  }


  delay(10);
}
#include <M5StickC.h>
#include <MadgwickAHRS.h>
Madgwick MadgwickFilter;

#define PI 3.141592
#define n 5
#define mm 10

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float temp = 0;

float ROLL=0, PITCH=0, YAW=0;

float smuki = 0, accXs[n],accZs[n],gyroYs[n],cmuki = 0,tmuki = 0;
int cnt, btn, i;

void setup() {
  // M5stickCの初期化
  M5.begin();
  // 液晶Lcdの初期化，設定
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("start");
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.println("  X       Y       Z");
  // 加速度センサMPU6886の初期化
  M5.MPU6886.Init();
  // フィルタ開始
  MadgwickFilter.begin(100); //100Hz
  // シリアル通信の開始9600bps
  // Serial.begin(9600);
  // ledcAttachPin(26, 0);
  // ledcSetup(0, 5000, 13);
}

int get_angle(){
  smuki = asin(accX)*180/PI;
  cmuki = acos(accZ)*180/PI;
  tmuki = atan2(accX, accZ)*180/PI;
  if (smuki>=-mm && smuki<=mm ){
    return 0;
  }
  if (cmuki>=-mm && cmuki<=mm){
    return 1;
  }
  }

int get_move(int btn){
  smuki = asin(accX)*180/PI;
  cmuki = acos(accZ)*180/PI;
  tmuki = atan2(accX, accZ)*180/PI;
    // ボタンが押されたら向きを検出
  if (btn == 1){
    if (cnt == 0){
      M5.Lcd.setCursor(20, 0);
      M5.Lcd.println("revel-1-get");

      cnt += 1;
    }
    if (cnt == 1 && get_angle()==1){
      M5.Lcd.setCursor(20, 0);
      M5.Lcd.println("revel-2-get");

      cnt += 1;
    }
    if (cnt == 2 && get_angle()==2){
      M5.Lcd.setCursor(20, 0);
      M5.Lcd.println("revel-3-get");

      cnt += 1;
    }
    if (cnt == 3 && get_angle()==3){
      M5.Lcd.setCursor(20, 0);
      M5.Lcd.println("revel-4-get");

      cnt += 1;
    }
    if (cnt == 4 && get_angle()==0){
      M5.Lcd.setCursor(20, 0);
      M5.Lcd.println("revel-5-get");

      cnt += 1;
    }
    if (cnt == 5 && get_angle()==2){
      M5.Lcd.setCursor(20, 0);
      M5.Lcd.println("Compreat");
      cnt=0;
      btn=0;
    }

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // MPU6886の値を取得
  M5.MPU6886.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.MPU6886.getAccelData(&accX,&accY,&accZ);
  M5.MPU6886.getTempData(&temp);
  M5.update();

  MadgwickFilter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);
  ROLL = MadgwickFilter.getRoll();
  PITCH = MadgwickFilter.getPitch();
  YAW   = MadgwickFilter.getYaw();

  // 重力加速度を1とした角度
  smuki = asin(accX)*180/PI;
  cmuki = acos(accZ)*180/PI;
  tmuki = atan2(accX, accZ)*180/PI;

  // ボタンBを押すと値を初期化
  if(M5.BtnB.wasReleased()){
    btn=0;
    for (i=0;i==5;i++){
      accXs[i]=0;
      accZs[i]=0;
      gyroYs[i]=0;
    }
    cnt=0;
  }
  // ボタンAを押すことで値の取得開始
  // if(M5.BtnA.wasReleased() && get_angle()==1 || get_angle()==0){
  //   M5.Lcd.fillScreen(ORANGE);
  //   btn = 1;
  //   M5.Lcd.setCursor(20, 0);
  //   M5.Lcd.println("Start-get-hurihuri");
  // }

  if(btn==0){
  // 液晶に描画
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ", smuki, cmuki,tmuki);
  M5.Lcd.setCursor(140, 30);
  M5.Lcd.print("deg");
  M5.Lcd.setCursor(0, 45);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ",accX * 1000,accY * 1000, accZ * 1000);
  M5.Lcd.setCursor(140, 45);
  M5.Lcd.print("mg");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ",ROLL ,PITCH, YAW);
  }

  // if (btn==1){
  //   M5.Lcd.setCursor(0, 15);
  //   M5.Lcd.printf("%.2f   %.2f   %.2f      ", accXs[0], accZs[0],gyroYs[0]);
  //   M5.Lcd.setCursor(0, 30);
  //   M5.Lcd.printf("%.2f   %.2f   %.2f      ", accXs[1], accZs[1],gyroYs[1]);
  //   M5.Lcd.setCursor(0, 40);
  //   M5.Lcd.printf("%.2f   %.2f   %.2f      ", accXs[2], accZs[2],gyroYs[2]);
  //   M5.Lcd.setCursor(0, 55);
  //   M5.Lcd.printf("%.2f   %.2f   %.2f      ", accXs[3], accZs[3],gyroYs[3]);
  //   M5.Lcd.setCursor(0, 70);
  //   M5.Lcd.printf("%.2f   %.2f   %.2f      ", accXs[4], accZs[4],gyroYs[4]);
  // }






  // シリアル通信を用いて書き込み
  // Serial.println(accX);
  // Serial.println(accY);
  // Serial.println(accZ);
  // Serial.println(gyroX);
  // Serial.println(gyroY);
  // Serial.println(gyroZ);
  // ledcWriteTone(0, 3136);
  delay(100);
}


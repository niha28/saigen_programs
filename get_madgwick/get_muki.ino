#include <M5StickC.h>
#include <MadgwickAHRS.h>
Madgwick MadgwickFilter;

#define e_gx 12
#define e_gy -5
#define e_gz -6

float accX = 0;
float accY = 0;
float accZ = 0;

float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;

float ROLL, PITCH, YAW;

float temp = 0;
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("madgwick");
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.println("  X       Y       Z");
  MadgwickFilter.begin(100);
  M5.MPU6886.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.MPU6886.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.MPU6886.getAccelData(&accX,&accY,&accZ);
  M5.MPU6886.getTempData(&temp);

  MadgwickFilter.updateIMU(gyroX+e_gx , gyroY+e_gy, gyroZ+e_gz, accX, accY, accZ);
  ROLL = MadgwickFilter.getRoll();
  PITCH = MadgwickFilter.getPitch();
  YAW  = MadgwickFilter.getYaw();

  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ", ROLL, PITCH, YAW);
  M5.Lcd.setCursor(140, 30);
  M5.Lcd.print("deg");
  M5.Lcd.setCursor(0, 45);
  M5.Lcd.printf("%.2f   %.2f   %.2f      ",accX * 1000,accY * 1000, accZ * 1000);
  M5.Lcd.setCursor(140, 45);
  M5.Lcd.print("mg");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.printf("Temperature : %.2f C", temp);
  delay(10);
}

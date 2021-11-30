import serial
import pandas as pd
import time

# シリアル通信のポート指定
ser = serial.Serial("COM7",9600,timeout=None)
cnt = 0
k = ""
# x,y,z軸の加速度を保存するリストを作成
lGx=[]
lGy=[]
lGz=[]
lAx=[]
lAy=[]
lAz=[]
sec=[]


while True:
    k = input("start:s\nquit:q\n")
    lGx=[]
    lGy=[]
    lGz=[]
    lAx=[]
    lAy=[]
    lAz=[]
    sec=[]
    if k == "s":
        ser.write(1)
        for t in range(50):
            ax = float(ser.readline().rstrip().decode(encoding="utf-8"))
            ay = float(ser.readline().rstrip().decode(encoding="utf-8"))
            az = float(ser.readline().rstrip().decode(encoding="utf-8"))
            gx = float(ser.readline().rstrip().decode(encoding="utf-8"))
            gy = float(ser.readline().rstrip().decode(encoding="utf-8"))
            gz = float(ser.readline().rstrip().decode(encoding="utf-8"))

            lAx.append(ax)
            lAy.append(ay)
            lAz.append(az)
            lGx.append(gx)
            lGy.append(gy)
            lGz.append(gz)
            sec.append(t*100)
            time.sleep(0.1)
        p=pd.DataFrame({
            "time[ms]":sec,
            "Gx[mg]":lGx,
            "Gy[mg]":lGy,
            "Gz[mg]":lGz,
            "Ax[dps]":lAx,
            "Ay[dps]":lAy,
            "Az[dps]":lAz
        })
        p.to_csv(f"out{cnt}.csv")
        print("Done")
        print(p)
        cnt += 1
    if k == "q":
        ser.close
        break


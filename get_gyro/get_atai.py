import serial
import pandas as pd
import time

# シリアル通信のポート指定
ser = serial.Serial("COM7",9600,timeout=None)
print("stert")

# x,y,z軸の加速度，角加速度を保存するリストを作成
lGx=[]
lGy=[]
lGz=[]
lAx=[]
lAy=[]
lAz=[]
sec=[]

# 5秒間値を取得
for t in range(50):
    # シリアル通信で値を読み込み
    ax = float(ser.readline().rstrip().decode(encoding="utf-8"))
    ay = float(ser.readline().rstrip().decode(encoding="utf-8"))
    az = float(ser.readline().rstrip().decode(encoding="utf-8"))
    gx = float(ser.readline().rstrip().decode(encoding="utf-8"))
    gy = float(ser.readline().rstrip().decode(encoding="utf-8"))
    gz = float(ser.readline().rstrip().decode(encoding="utf-8"))
    # リストに追加
    lAx.append(ax)
    lAy.append(ay)
    lAz.append(az)
    lGx.append(gx)
    lGy.append(gy)
    lGz.append(gz)
    sec.append(t*100)
    time.sleep(0.1)
# csvに書き込み
p=pd.DataFrame({
    "time[ms]":sec,
    "Gx[mg]":lGx,
    "Gy[mg]":lGy,
    "Gz[mg]":lGz,
    "Ax[dps]":lAx,
    "Ay[dps]":lAy,
    "Az[dps]":lAz
})
p.to_csv("nikome.csv")
print("Done")
print(p)
# シリアル通信終了
ser.close()
import serial
import pandas as pd
import time

ser = serial.Serial("COM7",9600,timeout=None)
print("stert")

datas = []
ls = []

while True:
    z = float(ser.readline())
    print(z)


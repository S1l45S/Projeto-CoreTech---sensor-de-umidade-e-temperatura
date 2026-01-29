import serial
import csv
from datetime import datetime

porta = "COM8"
baudrate = 9600

ser = serial.Serial(porta, baudrate)

with open("dados.csv", "a", newline="") as arquivo:
    writer = csv.writer(arquivo)

    while True:
        linha = ser.readline().decode().strip()
        dados = linha.split(";")

        if len(dados) == 2:
            temperatura, umidade = dados
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

            writer.writerow([timestamp, temperatura, umidade])
            print(timestamp, temperatura, umidade)

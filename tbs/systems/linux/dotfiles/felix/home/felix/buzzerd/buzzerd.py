import serial

SERIAL = "/dev/ttyACM0"
PIPE = "/dev/buzzer"

ser = serial.Serial(SERIAL, 115200)

print("buzzer daemon started")

while True:
    with open(PIPE, "r") as f:
        for line in f:
            ser.write((line.strip() + "\n").encode())
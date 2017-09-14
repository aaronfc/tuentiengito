import threading
import sys
import serial

serialPort = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)

def handleSerialIn(data):
    sys.stderr.write("sIn: "+data)

def writeToSerial(data):
    serialPort.write(data.encode('ascii'))

def readFromSerial(serialPort):
    thread = threading.currentThread()
    while getattr(thread, "running", True):
        serialIn = serialPort.readline().decode()
        if serialIn:
            handleSerialIn(serialIn)

thread = threading.Thread(target=readFromSerial, args=(serialPort,))
thread.start()

UP="MOVE_FORWARDS 3000 255"
DOWN="MOVE_BACKWARDS 3000 255"
RIGHT="TURN_RIGHT 3000 255"
LEFT="TURN_LEFT 3000 255"
NOOP="NOOP 1"

while True:
    line = input("$> ")
    if line == "w":
        writeToSerial(UP)
    if line == "a":
        writeToSerial(LEFT)
    if line == "s":
        writeToSerial(DOWN)
    if line == "d":
        writeToSerial(RIGHT)
    if line == " ":
        writeToSerial(NOOP)

    if line == 'quit':
        thread.running = False
        sys.exit()
    writeToSerial(line)

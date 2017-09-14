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

while True:
    line = input("$> ")
    if line == 'quit':
        thread.running = False
        sys.exit()
    writeToSerial(line)

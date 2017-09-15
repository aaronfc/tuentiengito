import threading
import sys
import serial
from time import sleep

serialPort = serial.Serial('/dev/ttyUSB0', 57600, timeout=1)

def handleSerialIn(data):
    sys.stderr.write("sIn: "+data)

def writeToSerial(data):
    serialPort.write(data.encode('ascii'))

def readFromSerial(serialPort):
    thread = threading.currentThread()
    while getattr(thread, "running", True):
        serialIn = serialPort.readline().decode('ascii')
        if serialIn:
            handleSerialIn(serialIn)

thread = threading.Thread(target=readFromSerial, args=(serialPort,))
thread.start()

UP="MOVE_FORWARDS 3000 255"
DOWN="MOVE_BACKWARDS 3000 255"
RIGHT="TURN_RIGHT 3000 255"
LEFT="TURN_LEFT 3000 255"
NOOP="NOOP 1"

def getCh():
    import sys, tty, termios
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

while True:
    line = getCh()
    print(line)
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

    if line == 'q':
        thread.running = False
        sys.exit()
    sleep(1/100)

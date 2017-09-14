from SerialChannel import SerialChannel
from random import randint
from time import sleep
import sys
import atexit


COLLISION_THRESHOLD = 10
serialResult = ""


def debug_handler(data):
    sys.stderr.write("sIn" + data)


def getSerial(data):
    serialResult = data


debug = True
if not debug:
    channel = SerialChannel('/dev/ttyUSB0')
    channel.addHandler(debug_handler)
    channel.addHandler(getSerial)


def writeToSerial(string):
    # TODO 
    if not debug:
        channel.write(string)
    else:
        print(string)


def readFromSerial():
    if not debug:
        return int(serialResult.split(':')[1]) < COLLISION_THRESHOLD

    return randint(1, 100) > 80


def forward():
    writeToSerial("MOVE_FORWARDS 10 255")


def backward():
    writeToSerial("MOVE_BACKWARDS 10 255")


def randomTurn():
    if (randint(1, 100) > 50):
        turnLeft()
    else:
        turnRight()


def turn():
    turnLeft()


def turnLeft():
    writeToSerial("TURN_LEFT " + repr(randint(10, 1000)))


def turnRight():
    writeToSerial("TURN_RIGHT " + repr(randint(10, 1000)))


def collision():
    return readFromSerial()


def loop():
    if (collision()):
        backward()
        turn()
        return

    if randint(1, 100) < 90:
        forward()
    else:
        randomTurn()


def exit_cleanup():
    channel.close()


if __name__ == '__main__':
    atexit.register(exit_cleanup)
    while True:
        try:
            # Main loop
            loop()
            sleep(1)
        except:
            # Cleanup
            exit_cleanup()

# from serialTest import writeToSerial,readFromSerial
from random import randint
from time import sleep


def writeToSerial(string):
    print(string)


def readFromSerial():
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


if __name__ == '__main__':
    while True:
        loop()
        sleep(1)

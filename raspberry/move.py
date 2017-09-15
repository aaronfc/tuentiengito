from SerialChannel import SerialChannel
from random import randint
from time import sleep,time
from say import say
import sys
import atexit
import random


COLLISION_THRESHOLD = 20
serialResult = "uninitialized"


def debug_handler(data):
    sys.stderr.write("sIn" + data)
    global serialResult
    serialResult = data
    sys.stderr.write("--" + serialResult)


debug = False
if not debug:
    channel = SerialChannel('/dev/ttyUSB0')
    channel.addHandler(debug_handler)


def writeToSerial(string):
    # TODO 
    if not debug:
        channel.write(string)
    print(string)


def readFromSerial():
    if not debug:
        global serialResult
        print("checking collision. SerialResult: " + serialResult)
        parts = serialResult.split(':')
        if len(parts) > 1:
            result = int(parts[1]) < COLLISION_THRESHOLD
            print("collision:" + repr(result))
            return result
        else:
            return False
    return randint(1, 10000) > 9990


last_forward = 0


def forward():
    global last_forward
    if int(time()) - last_forward > 3:
        last_forward = int(time())
        writeToSerial("MOVE_FORWARDS 5000 255")


def backward():
    global last_forward
    last_forward = 0
    writeToSerial("MOVE_BACKWARDS 5000 255")
    sleep(1)


def randomTurn():
    if (randint(1, 100) > 50):
        turnLeft()
    else:
        turnRight()


def turn():
    turnLeft()


def turnLeft():
    global last_forward
    last_forward = 0
    time_ = randint(500, 1500)
    writeToSerial("TURN_LEFT " + repr(time_))
    sleep(time_ / 1000)


def turnRight():
    global last_forward
    last_forward = 0
    time_ = randint(500, 1500)
    writeToSerial("TURN_RIGHT " + repr(time_))
    sleep(time_ / 1000)


def collision():
    return readFromSerial()


sentences = ["hostias", "quítate de enmedio", "me cago en la puta", "por aquí no", "uuuuy",
             "me cachis", "jopetas", "ouch", "ups"]


def yell():
    if randint(1, 10) > 8:
        say(random.choice(sentences))

def loop():
    if (collision()):
        yell()
        backward()
        turn()
        return
    forward()


def exit_cleanup():
    writeToSerial("NOOP 12")
    channel.close()


if __name__ == '__main__':
    atexit.register(exit_cleanup)
    while True:
        #try:
            # Main loop
        loop()
        sleep(0.1)
        #except:
            # Cleanup
           # exit_cleanup()

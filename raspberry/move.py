from random import randint
from time import sleep, time
from say import say
import sys
import atexit
import random
from sound import play


COLLISION_THRESHOLD = 20
serialResult = "uninitialized"
SPEED = repr(180)
TURN_MIN = 500
TURN_MAX = 1500


def debug_handler(data):
    global serialResult
    sys.stderr.write("--" + data)
    serialResult = data
    #sys.stderr.write("--" + serialResult)


debug = False
lights = False

if not debug:
    from SerialChannel import SerialChannel
    channel = SerialChannel('/dev/ttyUSB0')
    channel.addHandler(debug_handler)


############################################
############# SERIAL #############
############################################


def writeToSerial(string):
    # TODO 
    if not debug:
        channel.write(string)
    print(string)


def readFromSerial():
    if not debug:
        global serialResult
        #print("checking collision. SerialResult: " + serialResult)
        parts = serialResult.split(':')
        if len(parts) > 1:
            result = int(parts[1]) < COLLISION_THRESHOLD
            print("collision:" + repr(result))
            return result
        else:
            return False
    return randint(1, 10000) > 9990



################################################
################## LIGHTS ######################
################################################


LIGHT_OFF = "LEDS_OFF"
LIGHT_KNIGHTRIDER = "LEDS_KNIGHTRIDER"
LIGHT_ALARM = "LEDS_ALARM"
LIGHT_RAINBOW = "LEDS_RAINBOW"


def lightMode(mode):
    writeToSerial(mode)


############################################
######### ENGINE #############
############################################

last_forward = 0


def forward():
    global last_forward
    if int(time()) - last_forward > 3:
        last_forward = int(time())
        writeToSerial("MOVE_FORWARDS 5000 " + SPEED)
        lightMode(LIGHT_KNIGHTRIDER)


def backward():
    global last_forward
    last_forward = 0
    writeToSerial("MOVE_BACKWARDS 5000 " + SPEED)
    sleep(1.2)


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
    time_ = randint(TURN_MIN, TURN_MAX)
    writeToSerial("TURN_LEFT " + repr(time_))
    sleep(time_ / 1000)


def turnRight():
    global last_forward
    last_forward = 0
    time_ = randint(TURN_MIN, TURN_MAX)
    writeToSerial("TURN_RIGHT " + repr(time_))
    sleep(time_ / 1000)


def collision():
    return readFromSerial()


################################################
################## SPEAK ######################
################################################

sentences = ["hostias", "quítate de enmedio", "me cago en la puta", "por aquí no", "uuuuy",
             "me cachis", "jopetas", "ouch", "ups"]


def yell():
    if randint(1, 10) > 7:
        say(random.choice(sentences))


sounds = ["1.wav",
          "2.wav",
          "4.wav",
          "5.wav",
          "cadaminuto.wav"
          #"chanchanchan.mp3",
          #"chinchun.mp3",
          "enjuto_mojamuto_no_funciona_internet.mp3",
          "hasta-el-infinito-y-mas-alla.mp3",
          "hevisto.wav",
          "matame.wav",
          "pota.wav",
          #"tintontin.mp3",
          #"tiruriru.mp3"
          ]


def fart():
    if randint(1, 1000) < 5:
        print("pedo")
        file = "/home/pi/sounds/" + random.choice(sounds)
        play(file)
        lightMode(LIGHT_RAINBOW)


def loop():
    fart()
    if (collision()):
        lightMode(LIGHT_ALARM)
        yell()
        backward()
        turn()
        return
    forward()


def exit_cleanup():
    if not debug:
        writeToSerial("NOOP 12")
        lightMode(LIGHT_OFF)
        channel.close()


if __name__ == '__main__':
    atexit.register(exit_cleanup)
    while True:
        loop()
        sleep(0.02)

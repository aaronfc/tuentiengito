import subprocess
import os
import sys


def play(soundFile):
    print("play: " + soundFile)
    FNULL = open(os.devnull, 'w')
    subprocess.Popen(["play", soundFile],
                     stdout=FNULL,
                     stderr=subprocess.STDOUT)


if __name__ == '__main__':
    file = 'pedos.wav'
    if (len(sys.argv) > 1):
        file = sys.argv[1]
    play(file)
    print("OK")

import subprocess
import os
from time import sleep

def play(soundFile):
    FNULL = open(os.devnull, 'w')
    subprocess.Popen(["play", soundFile], stdout=FNULL, stderr=subprocess.STDOUT)

file = 'pedos.wav'
play(file)

print("OK")

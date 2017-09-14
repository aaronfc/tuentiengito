#!/usr/bin/python3

import subprocess
import os
import sys
from time import sleep

def play(soundFile):
    FNULL = open(os.devnull, 'w')
    subprocess.Popen(["play", soundFile], stdout=FNULL, stderr=subprocess.STDOUT)

file = 'pedos.wav'
if (len(sys.argv) > 1):
    file = sys.argv[1]

play(file)

print("OK")

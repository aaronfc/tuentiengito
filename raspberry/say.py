#!/usr/bin/python3

import subprocess
import os
import sys
from time import sleep

def textToStdin(text):
    read, write = os.pipe()
    uu = text.encode('utf8')
    os.write(write, uu)
    os.close(write)
    return read

def say(text):
    read = textToStdin(text)
    subprocess.Popen(["espeak", "-s", "190", "-v", "spanish"], stdin=read)


if __name__ == '__main__':
    text = "Juanjo, te amo!"
    if (len(sys.argv) > 1):
        text = sys.argv[1]
    say(text)
    print("OK")

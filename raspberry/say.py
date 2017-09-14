import subprocess
import os
from time import sleep

def textToStdin(text):
    read, write = os.pipe()
    uu = text.encode('utf8')
    os.write(write, uu)
    os.close(write)
    return read

def say(text):
    read = textToStdin(text)
    subprocess.run(["espeak", "-s", "190", "-v", "spanish"], stdin=read)


text = "Juanjo, te amo!"
say(text)
print("OK")

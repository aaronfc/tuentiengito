import threading
import atexit
import sys
import serial

class SerialChannel:
    def __init__(self, serialTty='/dev/ttyUSB0'):
        self._port = serial.Serial(serialTty, 9600, timeout=1)
        self._thread = threading.Thread(target=self.readFromSerial, args=(self._port,))
        self._thread.start()
        self._handlers = []
    
    def write(self, data):
        self._port.write(data.encode('ascii'))

    def addHandler(self, handler):
        self._handlers.append(handler)

    def close(self):
        self._thread.running = False

    def _handleSerialIn(self, data):
        for handler in self._handlers:
            handler(data)

    def _readFromSerial(self, port):
        thread = threading.currentThread()
        while getattr(thread, "running", True):
            serialIn = port.readline().decode()
            if serialIn:
                self._handleSerialIn(serialIn)

def debug_handler(data):
    sys.stderr.write("sIn"+data)

channel = SerialChannel('/dev/ttyUSB0')
channel.addHandler(debug_handler)

def exit_cleanup():
    channel.close()
atexit.register(exit_cleanup)
while True:
    try:
        #Main loop
    except:
        #Cleanup
        exit_cleanup()
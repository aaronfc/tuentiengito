import threading
import serial


class SerialChannel:
    def __init__(self, serialTty='/dev/ttyUSB0'):
        self._port = serial.Serial(serialTty, 9600, timeout=1)
        self._thread = threading.Thread(target=self._readFromSerial,
                                        args=(self._port,))
        self._thread.start()
        self._handlers = []

    def write(self, data):
        self._port.write((data + "\n").encode('ascii'))

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
            try:
                serialIn = port.readline().decode('ascii')
                if serialIn:
                    self._handleSerialIn(serialIn)
            except:
                pass # Ignored!

ARDUINO_DIR = /usr/share/arduino
ARDMK_DIR = /usr/share/arduino
AVR_TOOLS_DIR = /usr

BOARD_TAG = atmega328
ARDUINO_PORT = /dev/ttyUSB*
ARDUINO_LIBS = FastLED
OBJDIR = build
USER_LIB_PATH = ../lib
include /usr/share/arduino/Arduino.mk

update:
	pkill -9 python
	$(MAKE) upload
	/usr/bin/espeak  "Updated"

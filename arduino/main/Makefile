ARDUINO_DIR = /usr/share/arduino
ARDMK_DIR = /usr/share/arduino
AVR_TOOLS_DIR = /usr

BOARD_TAG = atmega328
ARDUINO_PORT = /dev/ttyUSB*
#ARDUINO_LIBS = SPI I2C pca9635 i2c_device
OBJDIR = build
include /usr/share/arduino/Arduino.mk

update:
	pkill -9 python
	$(MAKE) upload
	/usr/bin/espeak  "Updated"

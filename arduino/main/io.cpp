// IO Operations

#define BUFFER_DELAY 6
#include <Arduino.h>

namespace io {
  void sendEvent(String name, int value)
  {
    Serial.print(name);
    Serial.print(":");
    Serial.println(value);
    Serial.flush(); // Experimental
  }

  char* readLine() {
    char* readString = (char*) malloc(100);
    bool eof = false;
    int i = 0;
    while (Serial.available() && !eof && i < 99) {
      delay(BUFFER_DELAY);  //delay to allow buffer to fill
      if (Serial.available() > 0) {
        char c = Serial.read();  //gets one byte from serial buffer
        readString[i] = c; //makes the string readString
        if (c == ';' || c == '\n') {
          eof = true;
        }
        i++;
      }
    }
    readString[i] = '\0';
    return readString;
  }
}

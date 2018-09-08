#include "log.h"
#include <Arduino.h>

Log::Log() {
  this->enabled = false;
}
void Log::enable() {
  Serial.println("[ W A R N I N G ] Log enabled. Blocking 3 seconds...");
  delay(3);
  this->enabled = true;
}
Log& Log::d(const char str[]) {
  if(this->enabled) Serial.print(str);
  return *this;
}
Log& Log::d(const int value) {
  if(this->enabled) Serial.print(value);
  return *this;
}
Log& Log::d(const long value) {
  if(this->enabled) Serial.print(value);
  return *this;
}
Log& Log::d(const unsigned int value) {
  if(this->enabled) Serial.print(value);
  return *this;
}
Log& Log::d(const unsigned long value) {
  if(this->enabled) Serial.print(value);
  return *this;
}
Log& Log::eol() {
  if(this->enabled) Serial.println();
  return *this;
}

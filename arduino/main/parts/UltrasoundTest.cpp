#include <iostream>
#include <unistd.h>

int OUTPUT = 0;
int INPUT = 1;
int LOW = 0;
int HIGH = 1;

void pinMode(int pin, int mode) {
  std::cout << "Setting pin " << pin << " in mode " << mode << "\n";
}
void digitalWrite(int pin, int level) {
  std::cout << "Digital-writting to pin " << pin << " level " << level << "\n";
}
void delayMicroseconds(int microSeconds) {
  usleep(microSeconds);
}
long pulseIn(int pin, int level, unsigned long timeout) {
  long result = 42;
  std::cout << "pulseIn: " << result << "\n";
  return result;
}
// --- Test

#include "Ultrasound.cpp"

int main()
{
  Ultrasound *ultrasound1 = new Ultrasound(0, 1);
  ultrasound1->setup();
  while (true) {
    int distance = ultrasound1->getDistance();
    std::cout << "Distance: " << distance << "\n";
  }
  return 0;
}


//cout << distance << "\n";

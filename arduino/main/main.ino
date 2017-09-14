#include "parts/Ultrasound.cpp"

Ultrasound *ultrasound;
const int SAMPLE_PIN_1 = 1;

const String US_ID = "1";
const int US_TRIGGER_PIN = 9;
const int US_ECHO_PIN = 10;
const int US_COLLISION = 3;

void setup()
{
  Serial.begin(9600); // Serial default to 9600bps
  ultrasound = new Ultrasound(US_ID, 9600, US_TRIGGER_PIN, US_ECHO_PIN, US_COLLISION);
  ultrasound->setup();
}

void loop()
{
  // Read serial input
  // TBD

  // Execute commands
  // TBD

  // Read sensors
  ultrasound->read();

  // Write serial output
  // TBD
}

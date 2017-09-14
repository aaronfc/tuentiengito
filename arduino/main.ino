#include <parts/Sample.h>

Sample *sample;
const int SAMPLE_PIN_1 = 1;

void setup()
{
  Serial.begin(9600); // Serial default to 9600bps
  sample = new Sample(SAMPLE_PIN_1);
  sample->setup();
}

void loop()
{
  // Read serial input
  // TBD

  // Execute commands
  // TBD

  // Read sensors
  int output = sample->plusone(3); // output: 4

  // Write serial output
  // TBD
}

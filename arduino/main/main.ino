#include "parts/Ultrasound.cpp"

Ultrasound *ultrasound;
const int SAMPLE_PIN_1 = 1;

const int US_TRIGGER_PIN = 9;
const int US_ECHO_PIN = 10;

void setup()
{
  Serial.begin(9600); // Serial default to 9600bps
  //engineController = new EngineController(1,2,3,4,5);
  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();
}

void loop()
{
  // Read sensors
  int us1Distance = ultrasound1->getDistance();

  // Write serial output
  Serial.println("US1:" + us1Distance);

  // Read serial input
  //String command = engineController->readCommand();

  // Execute commands
  //engineController->executeCommand(command);
}

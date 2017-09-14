#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"

Ultrasound *ultrasound1;
EngineController *engineController;

const int US_TRIGGER_PIN = 9;
const int US_ECHO_PIN = 10;

void setup()
{
  Serial.begin(9600); // Serial default to 9600bps
  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();
  engineController = new EngineController();
  engineController->setup();
}

void loop()
{
  // Read sensors
  int us1Distance = ultrasound1->getDistance();

  // Write serial output
  Serial.println("US1:" + us1Distance);

  // EngineController
  char* command = engineController->readLine();
  engineController->executeCommand(command);
  free(command);
}

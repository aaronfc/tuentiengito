#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"

Ultrasound *ultrasound1;
int lastUs1Value;
EngineController *engineController;

const BAUD_RATE = 115200;

const int US_TRIGGER_PIN = 13;
const int US_ECHO_PIN = 12;

void setup()
{
  Serial.begin(BAUD_RATE);
  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();
  engineController = new EngineController();
  engineController->setup();
}

void sendEvent(String name, int value)
{
  Serial.print(name);
  Serial.print(":");
  Serial.println(value);
  Serial.flush(); // Experimental
}

void loop()
{
  // Read sensors
  int us1Distance = ultrasound1->getDistance();

  // Send events
  if (lastUs1Value != us1Distance) {
    lastUs1Value = us1Distance;
    sendEvent("US1", us1Distance);
  }

  // EngineController
  char* command = engineController->readLine();
  engineController->executeCommand(command);
  free(command);
}

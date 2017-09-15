#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"
#include "parts/LedStrip.cpp"
#include "FastRunningMedian.h"

Ultrasound *ultrasound1;
int lastUs1Value;
FastRunningMedian<unsigned int,5,30> us1Median;
EngineController *engineController;

// const long BAUD_RATE = 115200;  // 9600 for debug
const long BAUD_RATE = 9600;  // 9600 for debug

const int US_TRIGGER_PIN = 13;
const int US_ECHO_PIN = 12;

void setup()
{
  delay(3000); // power-up safety delay
  Serial.begin(BAUD_RATE);
  
  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();
  engineController = new EngineController();
  engineController->setup();

  initLedStrip();
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
  us1Median.addValue(ultrasound1->getDistance());
  int us1Distance = us1Median.getMedian();

  // Send events
  if (lastUs1Value != us1Distance) {
    lastUs1Value = us1Distance;
    sendEvent("US1", us1Distance);
  }

  // Commands
  char* command = engineController->readLine();

  // EngineController  
  if (command[0] != '\0') {    
    engineController->executeCommand(command);  
  } else {
    engineController->continueCommand();
  }

  // LedsController
  refreshLeds(command);
  
  // Free command
  free(command);
}

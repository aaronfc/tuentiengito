#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"
#include "parts/LedStrip.cpp"
#include "FastRunningMedian.h"

#define ENC_A 5
#define ENC_B 4

Ultrasound *ultrasound1;
int lastUs1Value;
FastRunningMedian<unsigned int,5,30> us1Median;
static EngineController *engineController;

// const long BAUD_RATE = 115200;  // 9600 for debug
const long BAUD_RATE = 57600;  // 9600 for debug

const int US_TRIGGER_PIN = 13;
const int US_ECHO_PIN = 12;

void setup()
{
  delay(3000); // power-up safety delay
  Serial.begin(BAUD_RATE);
  
  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();  
  engineController->setup(); 

  // Setup the encoders
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), upCounterA, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_B), upCounterB, FALLING);

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

void upCounterA() {
  if (engineController) {
    engineController->upCounterA();
  }
}

void upCounterB() {
  if (engineController) {
    engineController->upCounterB();
  }
}

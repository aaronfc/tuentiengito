#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"
#include "parts/LedStrip.cpp"
#include "parts/Engine.cpp"
#include "FastRunningMedian.h"
#include "io.cpp"

#define ENG_A_1 7
#define ENG_A_2 6
#define ENG_A_3 10
#define ENG_B_1 9
#define ENG_B_2 8
#define ENG_B_3 11

Ultrasound *ultrasound1;
int lastUs1Value;
FastRunningMedian<unsigned int,5,30> us1Median;
EngineController *engineController;

// const long BAUD_RATE = 115200;  // 9600 for debug
//const long BAUD_RATE = 57600;  // 9600 for debug
const long BAUD_RATE = 9600;

const int US_TRIGGER_PIN = 13;
const int US_ECHO_PIN = 12;

void setup()
{
  delay(3000); // power-up safety delay
  Serial.begin(BAUD_RATE);

  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();
  Engine* rightEngine = new Engine(ENG_A_1, ENG_A_2, ENG_A_3);
  Engine* leftEngine = new Engine(ENG_B_1, ENG_B_2, ENG_B_3);
  engineController = new EngineController(rightEngine, leftEngine);
  engineController->setup();

  initLedStrip();
}

void loop()
{
  // Read sensors

  us1Median.addValue(ultrasound1->getDistance());
  int us1Distance = us1Median.getMedian();

  // Send events
  if (lastUs1Value != us1Distance) {
    lastUs1Value = us1Distance;
    io::sendEvent("US1", us1Distance);
  }


  // Commands
  //char* input = io::readLine();
  // String command = Command::parse(input);
  //if (command != NULL) {
  //  if (engineController->understands(command)) {
  //    engineController->do(command);
  //  } else {
  //    engineController->idle();
  //  }
  //}

  // EngineController // @aaron: Previous behaviour
  char* command = io::readLine();
  if (command[0] != '\0') {
    engineController->executeCommand(command);
  } else {
    engineController->continueCommand();
  }

  // LedsController
  // refreshLeds(command); // @aaron: Delete?

  // Free command
  free(command);
}

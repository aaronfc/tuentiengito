#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"
#include "parts/Engine.cpp"
#include "FastRunningMedian.h"
//#include "io.cpp" // Deprecated
#include "tasks.cpp"
#include <Input.h>

// Engine pins
#define ENG_A_1 7
#define ENG_A_2 6
#define ENG_A_3 10
#define ENG_B_1 9
#define ENG_B_2 8
#define ENG_B_3 11

// Ultrasound pins
const int US_TRIGGER_PIN = 13;
const int US_ECHO_PIN = 12;

// SERIAL
const long BAUD_RATE = 9600;


Ultrasound *ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
int lastUs1Value = 0;
FastRunningMedian<unsigned int,5,30> us1Median;
EngineController *engineController;

// Tasks
Task *currentTask = 0;
Task *nextTask = 0;

// Actions and input commands setup
void moveForward(CommandParams &params, Stream &response) {
  int speed = params.getParamAsInt(0);
  int time = params.getParamAsInt(1);
  response.print("[INPUT] MOVE_FORWARDS speed:");
  response.print(speed);
  response.print(" time:");
  response.println(time);
  if (nextTask) { delete nextTask; }
  nextTask = new MoveForwardTask(engineController, speed, time);
}
void moveBackward(CommandParams &params, Stream &response) {
  int speed = params.getParamAsInt(0);
  int time = params.getParamAsInt(1);
  response.print("[INPUT] MOVE_BACKWARDS speed:");
  response.print(speed);
  response.print(" time:");
  response.println(time);
  if (nextTask) { delete nextTask; }
  nextTask = new MoveForwardTask(engineController, speed, time);
}
const InputCommand commandDefinitions[] PROGMEM = defineCommands(
  command("MOVE_FORWARDS", 2, &moveForward),
  command("MOVE_BACKWARDS", 2, &moveBackward)
);
Input input(60);

void setup()
{
  delay(3000); // power-up safety delay

  // Setup parts
  //ultrasound1->setup(); TODO Enable
  Engine* rightEngine = new Engine(ENG_A_1, ENG_A_2, ENG_A_3);
  Engine* leftEngine = new Engine(ENG_B_1, ENG_B_2, ENG_B_3);
  engineController = new EngineController(rightEngine, leftEngine);
  engineController->setup();

  input.begin(BAUD_RATE, commandDefinitions);
  Serial.println("Setup completed!");
}

void loop()
{
  // Read sensors

  //us1Median.addValue(ultrasound1->getDistance());
  //int us1Distance = us1Median.getMedian();

  // Send events
  //if (lastUs1Value != us1Distance) {
  //  lastUs1Value = us1Distance;
  //  oi::sendEvent("US1", us1Distance);
  //}

  // EngineController // @aaron: Previous behaviour
  //char* command = oi::readLine();
  //if (command[0] != '\0') {
  //  engineController->executeCommand(command);
  //} else {
  //  engineController->continueCommand();
  //}

  // Free command
  //free(command);

// Example: MOVE_FORWARDS 250 5000

  // Task Manager
  if (nextTask) { // New task!
      Serial.println("[TASKMANAGER] New task!");
      if (currentTask) { // Stop current task
        Serial.println("[TASKMANAGER] Stopping current task...");
        currentTask->stop();
        delete currentTask;
        Serial.println("[TASKMANAGER] Stopped task");
      }
      Serial.println("[TASKMANAGER] Starting new task...");
      currentTask = nextTask;
      currentTask->start();
      nextTask = 0;
  } else {
    if (currentTask) { // If already on a task:
        if (currentTask->keepGoing()) { // And has to keep running
          if (millis() % 1000 == 0) {
            Serial.println("[TASKMANAGER] Keep going...");
          }
          currentTask->update();
        } else { // Time to stop
          Serial.println("[TASKMANAGER] Completed task. Stopping...");
          currentTask->stop();
          delete currentTask;
          currentTask = 0;
          Serial.println("[TASKMANAGER] Stopped task");
        }
    }
  }
}

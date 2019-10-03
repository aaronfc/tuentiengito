#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"
#include "parts/Engine.cpp"
#include "FastRunningMedian.h"
#include "tasks.cpp"
#include <Input.h>
#include "log.h"

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
Log logger = Log();

// Tasks
Task *currentTask = 0;
Task *nextTask = 0;

// Actions and input commands setup
void moveForward(CommandParams &params, Stream &response) {
  int time = params.getParamAsInt(0);
  int speed = params.getParamAsInt(1);
  logger.d("[INPUT] MOVE_FORWARDS speed:").d(speed).d(" time:").d(time).eol();
  if (nextTask) { delete nextTask; }
  nextTask = new MoveForwardTask(engineController, speed, time);
}
void moveBackward(CommandParams &params, Stream &response) {
  int time = params.getParamAsInt(0);
  int speed = params.getParamAsInt(1);
  logger.d("[INPUT] MOVE_BACKWARDS speed:").d(speed).d(" time:").d(time).eol();
  if (nextTask) { delete nextTask; }
  nextTask = new MoveBackwardTask(engineController, speed, time);
}
void turnRight(CommandParams &params, Stream &response) {
  int time = params.getParamAsInt(0);
  logger.d("[INPUT] TURN RIGHT speed:").d(" time:").d(time).eol();
  if (nextTask) { delete nextTask; }
  nextTask = new TurnRightTask(engineController, time);
}
void turnLeft(CommandParams &params, Stream &response) {
  int time = params.getParamAsInt(0);
  logger.d("[INPUT] TURN LEFT speed:").d(" time:").d(time).eol();
  if (nextTask) { delete nextTask; }
  nextTask = new TurnLeftTask(engineController, time);
}
void stop(CommandParams &params, Stream &response) {
  logger.d("[INPUT] STOP").eol();
  if (nextTask) { delete nextTask; }
  if (currentTask) { 
    currentTask->stop();
    delete currentTask;
    currentTask = 0;
  }
}
void shake(CommandParams &params, Stream &response) {
  int time = params.getParamAsInt(0);
  logger.d("[INPUT] SHAKE").eol();
  if (nextTask) { delete nextTask; }
  nextTask = new ShakeTask(engineController, time);
}
const InputCommand commandDefinitions[] PROGMEM = defineCommands(
  command("MOVE_FORWARDS", 2, &moveForward),
  command("MOVE_BACKWARDS", 2, &moveBackward),
  command("TURN_RIGHT", 1, &turnRight),
  command("TURN_LEFT", 1, &turnLeft),
  command("TEMBLEQUE", 1, &shake),
  command("STOP", 0, &stop)
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
  // logger.enable();
}

void loop()
{
  // Read sensors

  //us1Median.addValue(ultrasound1->getDistance());
  //int us1Distance = us1Median.getMedian();

  // Send events
  //if (lastUs1Value != us1Distance) {
  //  lastUs1Value = us1Distance;
  //  sendEvent("US1", us1Distance);
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
      logger.d("[TASKMANAGER] New task!").eol();
      if (currentTask) { // Stop current task
        logger.d("[TASKMANAGER] Stopping current task...").eol();
        currentTask->stop();
        delete currentTask;
        logger.d("[TASKMANAGER] Stopped task").eol();
      }
      logger.d("[TASKMANAGER] Starting new task...").eol();
      currentTask = nextTask;
      currentTask->start();
      nextTask = 0;
  } else {
    if (currentTask) { // If already on a task:
        if (currentTask->keepGoing()) { // And has to keep running
          if (millis() % 1000 == 0) {
            logger.d("[TASKMANAGER] Keep going...").eol();
          }
          currentTask->update();
        } else { // Time to stop
          logger.d("[TASKMANAGER] Completed task. Stopping...").eol();
          currentTask->stop();
          delete currentTask;
          currentTask = 0;
          logger.d("[TASKMANAGER] Stopped task").eol();
        }
    }
  }
}


// IO

void sendEvent(String name, int value)
{
  Serial.print(name);
  Serial.print(":");
  Serial.println(value);
  Serial.flush(); // Experimental
}

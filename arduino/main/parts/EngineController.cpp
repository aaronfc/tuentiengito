#include "EngineController.h"

#ifndef INC_ENGINE_H
#include "Engine.h"
#endif

#define MAX_SPEED 255

EngineController::EngineController(Engine* rightEngine, Engine* leftEngine) {
  this->rightEngine = rightEngine;
  this->leftEngine = leftEngine;
}

void EngineController::setup() {
  rightEngine->setup();
  leftEngine->setup();
}

void EngineController::moveForward(int speed) {
  rightEngine->forwards(speed);
  leftEngine->forwards(speed);
}

void EngineController::moveBackward(int speed) {
  rightEngine->backwards(speed);
  leftEngine->backwards(speed);
}

void EngineController::moveTurningRight(int speed) {
  rightEngine->stop();
  leftEngine->forwards(speed);
}

void EngineController::moveTurningLeft(int speed) {
  rightEngine->forwards(speed);
  leftEngine->stop();
}

void EngineController::stop() {
  rightEngine->stop();
  leftEngine->stop();
}


/*
 * KEPT AS DOC OF COMMUNICATION PROTOCOL

Command* EngineController::parseCommand(char* readLine) {
  Command* command;
  char* commandWord = strtok(readLine, " \n;");
  if (strcmp(commandWord, "MOVE_FORWARDS") == 0) {
    command = new MoveForwardsCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "MOVE_BACKWARDS") == 0) {
    command = new MoveBackwardsCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "TURN_LEFT") == 0) {
    command = new MoveLeftCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "TURN_RIGHT") == 0) {
    command = new MoveRightCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "TEMBLEQUE") == 0) {
    command = new TemblequeCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "NOOP") == 0) {
    return new Command(NOOP);
  } else {
    return new Command(VOID_COMMAND);
  }

  char* parameters = strtok (NULL," \n");
  int i = 0;
  while (parameters != NULL) {
    strcpy(command->parameters[i], parameters);
    parameters = strtok(NULL, " \n;");
    i++;
  }
  command->parameters[i][0] = NULL;
  return new TimedCommand(command);
}
*/

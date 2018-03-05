#include "EngineController.h"

#ifndef INC_ENGINE_H
#include "Engine.h"
#endif

#define MAX_SPEED 255
#define BUFFER_DELAY 6

#define VOID_COMMAND -1
#define NOOP  0
#define MOVE_FORWARDS 1
#define MOVE_BACKWARDS 2
#define TURN_LEFT 3
#define TURN_RIGHT 4
#define TEMBLEQUE 5


class MoveForwardsCommand : public Command {
  public:
    
    MoveForwardsCommand(Engine* rightEngine, Engine* leftEngine): Command(MOVE_FORWARDS, rightEngine, leftEngine){};

    void run() {
      int speed = parseParameter(parameters[1]);      
      rightEngine->forwards(speed);
      leftEngine->forwards(speed);
    }
};

class MoveBackwardsCommand : public Command {
  public:
    
    MoveBackwardsCommand(Engine* rightEngine, Engine* leftEngine): Command(MOVE_BACKWARDS, rightEngine, leftEngine){};

    void run() {
      int speed = parseParameter(parameters[1]);      
      rightEngine->backwards(speed);
      leftEngine->backwards(speed);
    }
};


class MoveRightCommand : public Command {
  public:
    
    MoveRightCommand(Engine* rightEngine, Engine* leftEngine): Command(TURN_RIGHT, rightEngine, leftEngine){};

    void run() {
      rightEngine->forwards(MAX_SPEED);
      leftEngine->stop();
    }
};

class MoveLeftCommand : public Command {
  public:
    
    MoveLeftCommand(Engine* rightEngine, Engine* leftEngine): Command(TURN_LEFT, rightEngine, leftEngine){};

    void run() {
      rightEngine->stop();
      leftEngine->forwards(MAX_SPEED);
    }
};

EngineController::EngineController(Engine* rightEngine, Engine* leftEngine) {
  this->rightEngine = rightEngine;
  this->leftEngine = leftEngine;
}

void EngineController::setup() {
  rightEngine->setup();
  leftEngine->setup();
}

void EngineController::executeCommand(char* commandStr) {
  // Read sensors and notify via the serial port of any relevant situations
  // Read serial port for commands
  command = parseCommand(commandStr);
  if (command->opCode != VOID_COMMAND) {
    processCommand(command);
  }
  // delete command;
}

void EngineController::continueCommand() {    
  if (endTimestamp > 0) {
    if (millis() >= endTimestamp) {      
      stopEverything();
    } else if (command->opCode == TEMBLEQUE) {
      command->performTembleque();
    }
  }
}

char* EngineController::readLine() {
  char* readString = (char*) malloc(100);
  bool eof = false;
  int i = 0;
  while (Serial.available() && !eof) {
    delay(BUFFER_DELAY);  //delay to allow buffer to fill 
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString[i] = c; //makes the string readString
      if (c == ';' || c == '\n') {
        eof = true;
      }
      i++;
    }
  }
  readString[i] = '\0';
  return readString;
}

/*
 * PRIVATE METHODS
 */

Command* EngineController::parseCommand(char* readLine) {
  Command* command;
  char* commandWord;
  commandWord = strtok(readLine, " \n;");
  if (strcmp(commandWord, "MOVE_FORWARDS") == 0) {
    command = new MoveForwardsCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "MOVE_BACKWARDS") == 0) {
    command = new MoveBackwardsCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "TURN_LEFT") == 0) {
    command = new MoveLeftCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "TURN_RIGHT") == 0) {
    command = new MoveRightCommand(rightEngine, leftEngine);
  } else if (strcmp(commandWord, "TEMBLEQUE") == 0) {
    command = new Command(TEMBLEQUE, rightEngine, leftEngine);
  } else if (strcmp(commandWord, "NOOP") == 0) {
    command = new Command(NOOP, rightEngine, leftEngine);
    command->parameters[0][0] = NULL;
    return command;
  } else {
    command = new Command(VOID_COMMAND, rightEngine, leftEngine);
    command->parameters[0][0] = NULL;
    return command;    
  }

  commandWord = strtok (NULL," \n");
  int i = 0;
  while (commandWord != NULL) {
    strcpy(command->parameters[i], commandWord);
    commandWord = strtok(NULL, " \n;");
    i++;
  }
  command->parameters[i][0] = NULL;
  return command;
}

void EngineController::processCommand(Command* command) {
  endTimestamp = millis() + command->getDuration();
  command->run();
}

void EngineController::stopEverything() {  
  rightEngine->stop();
  leftEngine->stop();
  endTimestamp = 0;
}

/*************************************************************************/

Command::Command(byte opCode, Engine* rightEngine, Engine* leftEngine) {
  this->opCode = opCode;
  this->leftEngine = leftEngine;
  this->rightEngine = rightEngine;
};

void Command::run() {
  if (opCode == NOOP) {
    return;
  }
  int speed;
  switch (opCode) {
    case TEMBLEQUE:      
      setTemblequeEndTimestamp();
      performTembleque();
      break;
  }
}

int Command::getDuration() {
  return parseParameter(parameters[0]);
}

int Command::parseParameter(char* parameter) {
  int intParameter = 0;
  int i = 0;
  while (parameter[i] != NULL) {
    int currentFigure = parameter[i] - '0'; // Convert the numeric character to the corresponding number
    intParameter = intParameter * 10 + currentFigure;
    i++;
  }
  return intParameter;
}

void Command::performTembleque() {  
  if (millis() >= temblequeEndTimestamp) {     
    if (temblequeDirection) {
      rightEngine->forwards(MAX_SPEED);
      leftEngine->backwards(MAX_SPEED);
    } else {
      rightEngine->backwards(MAX_SPEED);
      leftEngine->forwards(MAX_SPEED);
    }
    temblequeDirection = !temblequeDirection;
    setTemblequeEndTimestamp();
  }
}

void Command::setTemblequeEndTimestamp() {
  temblequeEndTimestamp = millis() + 100;
}

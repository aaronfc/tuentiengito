#ifndef INC_ENGINECONTROLLER_H
#define INC_ENGINECONTROLLER_H

struct Command {
  int opCode;
  char parameters[10][30];
} command;

class EngineController
{
  public:
    EngineController();
    void setup();
    char* readLine();
    void executeCommand(char* str);
    void continueCommand();
  private:
    unsigned long endTimestamp;
    uint8_t temblequeDirection;
    unsigned long temblequeEndTimestamp;
    Command parseCommand(char* command);
    int parseParameter(char* parameter);
    void processCommand(Command command);
    void stopEverything();
    void setTemblequeEndTimestamp();
    void performTembleque();
    Command command;
};

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

#define ENG_A_1 7
#define ENG_A_2 6
#define ENG_A_3 10
#define ENG_B_1 9
#define ENG_B_2 8
#define ENG_B_3 11

EngineController::EngineController() {}

void EngineController::setup() {
  pinMode(ENG_A_1, OUTPUT);
  pinMode(ENG_A_2, OUTPUT);
  pinMode(ENG_A_3, OUTPUT);
  pinMode(ENG_B_1, OUTPUT);
  pinMode(ENG_B_2, OUTPUT);
  pinMode(ENG_B_3, OUTPUT);
}

void EngineController::executeCommand(char* commandStr) {
  // Read sensors and notify via the serial port of any relevant situations
  // Read serial port for commands
  command = parseCommand(commandStr);
  if (command.opCode != VOID_COMMAND) {
    processCommand(command);
  }
}

void EngineController::continueCommand() {    
  if (endTimestamp > 0) {
    if (millis() >= endTimestamp) {      
      stopEverything();
    } else if (command.opCode == TEMBLEQUE) {
      performTembleque();
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

Command EngineController::parseCommand(char* readLine) {
  Command command;    
  char* commandWord;
  commandWord = strtok(readLine, " \n;");
  if (strcmp(commandWord, "MOVE_FORWARDS") == 0) {
    command.opCode = MOVE_FORWARDS;
  } else if (strcmp(commandWord, "MOVE_BACKWARDS") == 0) {
    command.opCode = MOVE_BACKWARDS;
  } else if (strcmp(commandWord, "TURN_LEFT") == 0) {
    command.opCode = TURN_LEFT;
  } else if (strcmp(commandWord, "TURN_RIGHT") == 0) {
    command.opCode = TURN_RIGHT;
  } else if (strcmp(commandWord, "TEMBLEQUE") == 0) {
    command.opCode = TEMBLEQUE;
  } else if (strcmp(commandWord, "NOOP") == 0) {
    command.opCode = NOOP;
    command.parameters[0][0] = NULL;
    return command;
  } else {
    command.opCode = VOID_COMMAND;
    command.parameters[0][0] = NULL;
    return command;    
  }

  commandWord = strtok (NULL," \n");
  int i = 0;
  while (commandWord != NULL) {
    strcpy(command.parameters[i], commandWord);
    commandWord = strtok(NULL, " \n;");
    i++;
  }
  command.parameters[i][0] = NULL;
  return command;
}

int EngineController::parseParameter(char* parameter) {
  int intParameter = 0;
  int i = 0;
  while (parameter[i] != NULL) {
    int currentFigure = parameter[i] - '0'; // Convert the numeric character to the corresponding number
    intParameter = intParameter * 10 + currentFigure;
    i++;
  }
  return intParameter;
}

void EngineController::processCommand(Command command) {
  if (command.opCode == NOOP) {
    return;
  }
  int speed;
  unsigned long time = parseParameter(command.parameters[0]);
  endTimestamp = millis() + time;  
  switch (command.opCode) {
    case MOVE_FORWARDS:
      digitalWrite (ENG_A_1, HIGH);
      digitalWrite (ENG_A_2, LOW);
      speed = parseParameter(command.parameters[1]);      
      analogWrite (ENG_A_3, speed); 
      digitalWrite (ENG_B_1, HIGH);
      digitalWrite (ENG_B_2, LOW);
      analogWrite (ENG_B_3, speed); 
      break;
    case MOVE_BACKWARDS:
      digitalWrite (ENG_A_1, LOW);
      digitalWrite (ENG_A_2, HIGH);
      speed = parseParameter(command.parameters[1]);
      analogWrite (ENG_A_3, speed); 
      digitalWrite (ENG_B_1, LOW);
      digitalWrite (ENG_B_2, HIGH);
      analogWrite (ENG_B_3, speed); 
      break;
    case TURN_LEFT:
      digitalWrite (ENG_A_1, LOW);
      digitalWrite (ENG_A_2, LOW);
      analogWrite (ENG_A_3, MAX_SPEED);
      digitalWrite (ENG_B_1, HIGH);
      digitalWrite (ENG_B_2, LOW);
      analogWrite (ENG_B_3, MAX_SPEED);
      break;
    case TURN_RIGHT:
      digitalWrite (ENG_A_1, HIGH);
      digitalWrite (ENG_A_2, LOW);
      analogWrite (ENG_A_3, MAX_SPEED);
      digitalWrite (ENG_B_1, LOW);
      digitalWrite (ENG_B_2, LOW);
      analogWrite (ENG_B_3, MAX_SPEED);
      break;
    case TEMBLEQUE:      
      setTemblequeEndTimestamp();
      performTembleque();
      break;
  }
}

void EngineController::stopEverything() {  
  digitalWrite (ENG_A_1, LOW);
  digitalWrite (ENG_A_2, LOW);
  analogWrite (ENG_A_3, 0);
  digitalWrite (ENG_B_1, LOW);
  digitalWrite (ENG_B_2, LOW);
  analogWrite (ENG_B_3, 0);
  endTimestamp = 0;
}

void EngineController::performTembleque() {  
  if (millis() >= temblequeEndTimestamp) {        
    digitalWrite (ENG_A_1, temblequeDirection? HIGH: LOW);
    digitalWrite (ENG_A_2,  temblequeDirection? LOW: HIGH);
    analogWrite (ENG_A_3, MAX_SPEED);
    digitalWrite (ENG_B_1, temblequeDirection? LOW: HIGH);
    digitalWrite (ENG_B_2, temblequeDirection? HIGH: LOW);
    analogWrite (ENG_B_3, MAX_SPEED);
    temblequeDirection = !temblequeDirection;
    setTemblequeEndTimestamp();
  }
}

void EngineController::setTemblequeEndTimestamp() {
  temblequeEndTimestamp = millis() + 500;
}

#define BUFFER_DELAY 6
#define MAX_SPEED 255

#define VOID_COMMAND -1
#define NOOP  0
#define MOVE_FORWARDS 1
#define MOVE_BACKWARDS 2
#define TURN_LEFT 3
#define TURN_RIGHT 4


#define ENG_A_1 7
#define ENG_A_2 6
#define ENG_A_3 10
#define ENG_B_1 9
#define ENG_B_2 8
#define ENG_B_3 11

unsigned long endTimestamp;

struct Command {
  int opCode;
  char parameters[10][30];
} command;

void setup() {
  Serial.begin(9600);
  pinMode(ENG_A_1, OUTPUT);
  pinMode(ENG_A_2, OUTPUT);
  pinMode(ENG_A_3, OUTPUT);
  pinMode(ENG_B_1, OUTPUT);
  pinMode(ENG_B_2, OUTPUT);
  pinMode(ENG_B_3, OUTPUT);
}

void loop() {
  // Read sensors and notify via the serial port of any relevant situations
  // Read serial port for commands
  char* line = readLine();
  command= parseCommand(line);
  free(line);
  if (command.opCode != VOID_COMMAND) {
    processCommand(command);
  }
  if (endTimestamp > 0 && millis() >= endTimestamp) {
    stopEverything();
  }
}

char* readLine() {
  char* readString = (char*) malloc(100);
  bool eof = false;
  int i = 0;
  while (Serial.available() && !eof) {
    delay(BUFFER_DELAY);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
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

Command parseCommand(char* readLine) {
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
  } else if (strcmp(commandWord, "NOOP") == 0) {
    command.opCode = NOOP;
    command.parameters[0][0] = '\0';
    return command;
  } else {
    command.opCode = VOID_COMMAND;
    command.parameters[0][0] = '\0';
    return command;    
  }

  commandWord = strtok (NULL," \n;");
  int i = 0;
  while (commandWord != NULL) {
    strcpy(command.parameters[i], commandWord);
    commandWord = strtok(NULL, " \n;");
    i++;
  }
  command.parameters[i][0] = '\0';
  return command;  
}

int parseParameter(char* parameter) {
  int intParameter = 0;
  int i = 0;
  while (parameter[i] != '\0') {
    int currentFigure = parameter[i] - '0'; // Convert the numeric character to the corresponding number
    intParameter = intParameter * 10 + currentFigure;
    i++;
  }
  return intParameter;
}

void processCommand(Command command) {
  int speed;
  unsigned long time = parseParameter(command.parameters[0]);
  endTimestamp = millis() + time;
  Serial.println(command.opCode);
  switch (command.opCode) {
    case MOVE_FORWARDS:
      digitalWrite (ENG_A_1, HIGH);
      digitalWrite (ENG_A_2, LOW);
      speed = parseParameter(command.parameters[1]);      
      Serial.println(speed);
      analogWrite (ENG_A_3, speed); //Velocidad motor A
      //analogWrite (ENG_A_3, (int)(speed)); //Velocidad motor A
      digitalWrite (ENG_B_1, HIGH);
      digitalWrite (ENG_B_2, LOW);
      analogWrite (ENG_B_3, speed); //Velocidad motor B
      break;
    case MOVE_BACKWARDS:
      digitalWrite (ENG_A_1, LOW);
      digitalWrite (ENG_A_2, HIGH);
      speed = parseParameter(command.parameters[1]);
      Serial.println(speed);
      //analogWrite (ENG_A_3, speed*0.95); //Velocidad motor A
      analogWrite (ENG_A_3, speed); //Velocidad motor A
      digitalWrite (ENG_B_1, LOW);
      digitalWrite (ENG_B_2, HIGH);
      analogWrite (ENG_B_3, speed); //Velocidad motor B
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
  }
}

void stopEverything() {
  digitalWrite (ENG_A_1, LOW);
  digitalWrite (ENG_A_2, LOW);
  analogWrite (ENG_A_3, 0);
  digitalWrite (ENG_B_1, LOW);
  digitalWrite (ENG_B_2, LOW);
  analogWrite (ENG_B_3, 0);
}


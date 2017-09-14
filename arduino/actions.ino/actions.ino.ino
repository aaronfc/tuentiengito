#define BUFFER_DELAY 6

#define VOID_COMMAND -1
#define MOVE_FORWARDS 1
#define MOVE_BACKWARDS 2

#define ENG_A_1 7
#define ENG_A_2 6
#define ENG_B_1 9
#define ENG_B_2 8

struct Command {
  int opCode;
  char parameters[10][30];
} command;

void setup() {
  Serial.begin(9600);
  pinMode(ENG_A_1, OUTPUT);
  pinMode(ENG_A_2, OUTPUT);
  pinMode(ENG_B_1, OUTPUT);
  pinMode(ENG_B_2, OUTPUT);
}

void loop() {
  // Read sensors and notify via the serial port of any relevant situations
  // Read serial port for commands
  char* line = readLine();
  command = parseCommand(line);
  free(line);
  if (command.opCode != VOID_COMMAND) {    
    int i = 0;
    while (command.parameters[i][0] != '\0') {
      Serial.println(command.parameters[i]);
      i++;
    }
    processCommand(command);
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

void processCommand(Command command) {
  Serial.println(command.opCode);
  switch (command.opCode) {
    case MOVE_FORWARDS:
      digitalWrite (ENG_A_1, HIGH);
      digitalWrite (ENG_A_2, LOW);
      //analogWrite (ENA, 255); //Velocidad motor A
      digitalWrite (ENG_B_1, HIGH);
      digitalWrite (ENG_B_2, LOW);
      //analogWrite (ENB, 255); //Velocidad motor B
      break;
    case MOVE_BACKWARDS:
      digitalWrite (ENG_A_1, LOW);
      digitalWrite (ENG_A_2, HIGH);
      //analogWrite (ENA, 255); //Velocidad motor A
      digitalWrite (ENG_B_1, LOW);
      digitalWrite (ENG_B_2, HIGH);
      //analogWrite (ENB, 255); //Velocidad motor B
      break;
  }
}


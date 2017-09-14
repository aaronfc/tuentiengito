#define BUFFER_DELAY 6

#define VOID_COMMAND -1

#define MOVE_FORWARDS 1
#define MOVE_BACKWARDS 2

struct Command {
  int opCode;
  char parameters[10][30];
} command;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read sensors and notify via the serial port of any relevant situations
  // Read serial port for commands
  char* line = readLine();
  command = parseCommand(line);
  free(line);
  if (command.opCode != VOID_COMMAND) {
    Serial.println(command.opCode);
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
  Serial.println(commandWord);
  int i = 0;
  while (commandWord != NULL) {
    strcpy(commandWord, command.parameters[i]);
    commandWord = strtok(NULL, " \n;");
    i++;
  }
  command.parameters[i][0] = '\0';
  return command;  
}


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
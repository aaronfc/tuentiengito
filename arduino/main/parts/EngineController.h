#ifndef INC_ENGINECONTROLLER_H
#define INC_ENGINECONTROLLER_H

#ifndef INC_ENGINE_H
#include "Engine.h"
#endif

class Command {
  public:
    Command(byte opCode, Engine* rightEngine, Engine* leftEngine);
    virtual void run();
    void performTembleque();
    int getDuration();
    byte _opCode;
    char _parameters[10][30];
  protected:
    Engine* _rightEngine;
    Engine* _leftEngine;
    int parseParameter(char* parameter);
  private:
    uint8_t temblequeDirection;
    unsigned long temblequeEndTimestamp;
    void setTemblequeEndTimestamp();
};

class EngineController
{
  public:
    EngineController(Engine* rightEngine, Engine* leftEngine);
    void setup();
    char* readLine();
    void executeCommand(char* str);
    void continueCommand();
  private:
    unsigned long endTimestamp;
    Command* parseCommand(char* command);
    void processCommand(Command* command);
    void stopEverything();
    Command* command;
    Engine* _rightEngine;
    Engine* _leftEngine;
};

#endif
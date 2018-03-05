#ifndef INC_ENGINECONTROLLER_H
#define INC_ENGINECONTROLLER_H

#ifndef INC_ENGINE_H
#include "Engine.h"
#endif

class Command {
  public:
    Command(byte opCode, Engine* rightEngine, Engine* leftEngine);
    virtual void run();
    virtual void stop();
    virtual void continueCommand();
    int getDuration();
    byte opCode;
    char parameters[10][30];
  protected:
    Engine* rightEngine;
    Engine* leftEngine;
    int parseParameter(char* parameter);
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
    Engine* rightEngine;
    Engine* leftEngine;
};

#endif
#ifndef INC_ENGINECONTROLLER_H
#define INC_ENGINECONTROLLER_H

struct Command {
	  int opCode;
	    char parameters[10][30];
} command;

class EngineController {
  public:
    void setup();
    char* readLine();
    void executeCommand(char* str);
    void continueCommand();
    static EngineController& instance();
    static void isrEncoderA();
    static void isrEncoderB();
  protected:
    void upCounterA();
    void upCounterB();
  private:
    EngineController(){
      counterA = 0;
      counterB = 0;
    };
    EngineController(const EngineController&){};
    EngineController& operator=(const EngineController&){};

    unsigned long endTimestamp;
    uint8_t temblequeDirection;
    unsigned long temblequeEndTimestamp;
    Command parseCommand(char* command);
    int parseParameter(char* parameter);
    void processCommand(Command command);
    void stopEverything();
    void setTemblequeEndTimestamp();
    void performTembleque();
    void resetCounters();
    Command command;
    int counterA;
    int counterB;
};

#endif


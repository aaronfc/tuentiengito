#ifndef INC_ENGINECONTROLLER_H
#define INC_ENGINECONTROLLER_H

#ifndef INC_ENGINE_H
#include "Engine.h"
#endif

class EngineController
{
  public:
    EngineController(Engine* rightEngine, Engine* leftEngine);
    void setup();
    void moveForward(int speed);
    void moveBackward(int speed);
    void moveTurningRight(int speed);
    void moveTurningLeft(int speed);
    void stop();
  private:
    Engine* rightEngine;
    Engine* leftEngine;
};

#endif

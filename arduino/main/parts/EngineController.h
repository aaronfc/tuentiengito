#ifndef INC_ENGINECONTROLLER_H
#define INC_ENGINECONTROLLER_H

#ifndef INC_ENGINE_H
#include "Engine.h"
#endif

#define MAX_SPEED 100

class EngineController
{
  public:
    EngineController(Engine* rightEngine, Engine* leftEngine);
    void setup();
    void moveForward(int speed);
    void moveBackward(int speed);
    void moveTurningRight(int speed);
    void moveTurningLeft(int speed);
    void sharpTurningLeft(int speed);
    void sharpTurningRight(int speed);
    void stop();
  private:
    Engine* rightEngine;
    Engine* leftEngine;
};

#endif

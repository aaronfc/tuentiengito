#include <Arduino.h>
#ifndef INC_ENGINECONTROLLER_H
#include "parts/EngineController.h"
#endif


class Task {
  // Lifecycle:
  // start();
  // while (keepGoing) { update(); }
  // stop();
  public:
    virtual void start();
    virtual bool keepGoing();
    virtual void update();
    virtual void stop();    
};

// Movement tasks

class MovementTask : public Task {
  private:
    unsigned int time;
    unsigned long endTime;
  public:
    MovementTask(EngineController* ec, unsigned int time): Task() {
      this->ec = ec;
      this->time = time;
    }
    void start() {
      // Serial.println("[" + typeid(this).name() + "] Starting...");
      this->endTime = millis() + this->time;      
      this->perform();
      // Serial.println("[" + typeid(this).name() + "] Started");
    }
    bool keepGoing() {
      Serial.print(this->endTime, 10);
      Serial.print(" ");
      Serial.println(millis(), 10);
      return millis() < this->endTime;
    }
    void update() {} // Do nothing
    void stop() {
      this->ec->stop();
    }
  protected:
    EngineController* ec;
    virtual void perform();
};

class MoveForwardTask : public MovementTask {
  private:
    int speed;
  public:
    MoveForwardTask(EngineController* ec, int speed, unsigned int time): MovementTask(ec, time) {
      this->speed = speed;
    }
    void update() {} // Do nothing
    void stop() {
      Serial.println("[MoveForwardTask] Stopping...");
      MovementTask::stop();
      Serial.println("[MoveForwardTask] Stopped");
    }
  protected:
    void perform() {
      this->ec->moveForward(this->speed);
    }
};

class MoveBackwardTask : public MovementTask {
  private:
    int speed;
  public:
    MoveBackwardTask(EngineController* ec, int speed, unsigned int time): MovementTask(ec, time) {
      this->speed = speed;
    }
    void stop() {
      Serial.println("[MoveBackwardTask] Stopping...");
      MovementTask::stop();
      Serial.println("[MoveBackwardTask] Stopped");
    }
  protected:
    void perform() {
      this->ec->moveBackward(this->speed);
    }
};

class TurnRightTask : public MovementTask {
  public:
    TurnRightTask(EngineController* ec, unsigned int time): MovementTask(ec, time){};
    void stop() {
      Serial.println("[TurnRightTask] Stopping...");
      MovementTask::stop();
      Serial.println("[TurnRightTask] Stopped");
    }
  protected:
    void perform() {
      this->ec->moveTurningRight(MAX_SPEED);
    }
};

class TurnLeftTask : public MovementTask {
  public:
    TurnLeftTask(EngineController* ec, unsigned int time): MovementTask(ec, time){};
    void stop() {
      Serial.println("[TurnLeftTask] Stopping...");
      MovementTask::stop();
      Serial.println("[TurnLeftTask] Stopped");
    }
  protected: 
    void perform() {
      this->ec->moveTurningLeft(MAX_SPEED);
    }
};

class ShakeTask : public MovementTask {
  private:
    uint8_t shakeDirection;
    unsigned long shakeEndTimestamp;

    void setShakeEndTimestamp() {
      shakeEndTimestamp = millis() + 100;
    }

  public:
    ShakeTask(EngineController* ec, unsigned int time): MovementTask(ec, time){};

  protected:
    void update() {  
      if (millis() >= shakeEndTimestamp) {     
        if (shakeDirection) {
          this->ec->sharpTurningLeft(MAX_SPEED);
        } else {
          this->ec->sharpTurningRight(MAX_SPEED);
        }
        shakeDirection = !shakeDirection;
        setShakeEndTimestamp();
      }
    }
    void perform() {
      setShakeEndTimestamp();      
    }
};
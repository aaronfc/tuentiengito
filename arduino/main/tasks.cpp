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
    int time;
    int endTime;
  public:
    MovementTask(EngineController* ec, int time): Task() {
      this->ec = ec;
      this->time = time;
    }
    void start() {
      this->perform();
      this->endTime = millis() + this->time;
    }
    bool keepGoing() {
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
    MoveForwardTask(EngineController* ec, int speed, int time): MovementTask(ec, time) {
      this->speed = speed;
    }
    void start() {
      Serial.println("[MoveForwardTask] Starting...");
      MovementTask::start();
      Serial.println("[MoveForwardTask] Started");
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
    MoveBackwardTask(EngineController* ec, int speed, int time): MovementTask(ec, time) {
      this->speed = speed;
    }
    void start() {
      Serial.println("[MoveBackwardTask] Starting...");
      MovementTask::start();
      Serial.println("[MoveBackwardTask] Started");
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
    TurnRightTask(EngineController* ec, int time): MovementTask(ec, time){};
    void start() {
      Serial.println("[TurnRightTask] Starting...");
      MovementTask::start();
      Serial.println("[TurnRightTask] Started");
    }
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
    TurnLeftTask(EngineController* ec, int time): MovementTask(ec, time){};
    void start() {
      Serial.println("[TurnLeftTask] Starting...");
      MovementTask::start();
      Serial.println("[TurnLeftTask] Started");
    }
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
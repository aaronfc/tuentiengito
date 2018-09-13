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

class MoveForwardTask : public Task {
  private:
    int speed;
    int time;
    EngineController* ec;
    unsigned long endTime;
  public:
    MoveForwardTask(EngineController* ec, int speed, int time): Task() {
      this->ec = ec;
      this->speed = speed;
      this->time = time;
    }
    void start() {
      Serial.println("[MoveForwardTask] Starting...");
      // Engage engines in forward direction
      this->ec->moveForward(this->speed);
      this->endTime = millis() + this->time;
      Serial.println("[MoveForwardTask] Started");
    }
    bool keepGoing() {
        return millis() < this->endTime;
    }
    void update() {} // Do nothing
    void stop() {
      Serial.println("[MoveForwardTask] Stopping...");
      this->ec->stop();
      Serial.println("[MoveForwardTask] Stopped");
    }
};

class MoveBackwardsTask : public Task {
  private:
    int speed;
    int time;
    EngineController* ec;
    int endTime;
  public:
    MoveBackwardsTask(EngineController* ec, int speed, int time): Task() {
      this->ec = ec;
      this->speed = speed;
      this->time = time;
    }
    void start() {
      // Engage engines in forward direction
      this->ec->moveBackward(this->speed);
      this->endTime = millis() + this->time;
    }
    bool keepGoing() {
        return millis() < this->endTime;
    }
    void update() {} // Do nothing
    void stop() {
      this->ec->stop();
    }
};

class TurnTask : public Task {
  private:
    int time;
    int endTime;
  public:
    TurnTask(EngineController* ec, int time): Task() {
      this->ec = ec;
      this->time = time;
    }
    void start() {
      this->turn();
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
    virtual void turn();
};

class TurnRightTask : public TurnTask {
  public:
    TurnRightTask(EngineController* ec, int time): TurnTask(ec, time){};
  protected:
    void turn() {
      this->ec->moveTurningRight(MAX_SPEED);
    }
};

class TurnLeftTask : public TurnTask {
  public:
    TurnLeftTask(EngineController* ec, int time): TurnTask(ec, time){};
  protected: 
    void turn() {
      this->ec->moveTurningLeft(MAX_SPEED);
    }
};
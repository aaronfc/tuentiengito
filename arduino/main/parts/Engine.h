#ifndef INC_ENGINE_H
#define INC_ENGINE_H

class Engine {
  public: 
    Engine(byte controlPinA, byte controlPinB, byte powerPin) {
        _controlPinA = controlPinA;
        _controlPinB = controlPinB;
        _powerPin = powerPin;
    }
    void setup();
    void forwards(byte speed);
    void backwards(byte speed);
    void stop();
  private:
    byte _controlPinA;
    byte _controlPinB;
    byte _powerPin;
};

#endif
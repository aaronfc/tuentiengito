#ifndef INC_ULTRASOUND_H
#define INC_ULTRASOUND_H

class Ultrasound
{
  public:
    Ultrasound(int _triggerPin, int _echoPin);
    ~Ultrasound();
    void setup();
    int getDistance();
  private:
    int triggerPin;
    int echoPin;
};

#endif

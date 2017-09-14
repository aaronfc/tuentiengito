#ifndef SAMPLE_H
#define SAMPLE_H

#include <Arduino.h>

class Ssample {
public:
    Sample(int pin1);
    ~Sample();
    void setup();
    int plusone(int num);
private:
    int pin1;
    int sum(int a, int b)
};

#endif

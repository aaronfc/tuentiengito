#include "Ultrasound.h" //include the declaration for this class

const float SPEED = 10.0 / 292.0 / 2.0; // cm/μs
const float MIN_DIST = 30.0; // cm

//<<constructor>>
Ultrasound::Ultrasound(int _triggerPin, int _echoPin) {
    triggerPin = _triggerPin;
    echoPin = _echoPin;
}

//<<destructor>>
Ultrasound::~Ultrasound(){/*nothing to destruct*/}

void Ultrasound::setup() {
  pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

int Ultrasound::getDistance() {
  // Clears the trigPin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(4);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, MIN_DIST * 1.5 / SPEED);

  int distance = MIN_DIST;
  if (duration > 0) {
    // Calculating the distance
    distance = duration * SPEED;
  }
  return distance;
}

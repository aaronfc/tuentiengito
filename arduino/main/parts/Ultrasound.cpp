#include "Ultrasound.h" //include the declaration for this class

const float SPEED = 10.0 / 292.0 / 2.0; // cm/μs
const float MIN_DIST = 20.0; // cm

String id;
int speed;
int triggerPin;
int echoPin;
int collisionDistance;

//<<constructor>>
Ultrasound::Ultrasound(String _id, int _speed, int _triggerPin, int _echoPin, int _collisionDistance) {
    id = _id;
    speed = _speed;
    triggerPin = _triggerPin;
    echoPin = _echoPin;
    collisionDistance = _collisionDistance;
}

//<<destructor>>
Ultrasound::~Ultrasound(){/*nothing to destruct*/}

void Ultrasound::setup() {
  pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.println(id);
}

void Ultrasound::read() {
  // Clears the trigPin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(4);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, MIN_DIST * 1.5 / SPEED);

  if (duration > 0) {
    // Calculating the distance
    int distance = duration * SPEED;
    
    if (distance <= collisionDistance) {
      Serial.println("[US-" + id + "] COLLISION");
    }
  }
}

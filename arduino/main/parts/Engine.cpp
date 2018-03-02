#include "Engine.h"

void Engine::setup() {
  pinMode(_controlPinA, OUTPUT);
  pinMode(_controlPinB, OUTPUT);
  pinMode(_powerPin, OUTPUT);
}

void Engine::forwards(byte speed) {
  digitalWrite (_controlPinA, HIGH);
  digitalWrite (_controlPinB, LOW);
  analogWrite (_powerPin, speed); 
}

void Engine::backwards(byte speed) {
  digitalWrite (_controlPinA, LOW);
  digitalWrite (_controlPinB, HIGH);
  analogWrite (_powerPin, speed); 
}

void Engine::stop() {
  digitalWrite (_controlPinA, LOW);
  digitalWrite (_controlPinB, LOW);
  analogWrite (_powerPin, 0);
}

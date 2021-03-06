#include "parts/Ultrasound.cpp"
#include "parts/EngineController.cpp"
#include "parts/LedStrip.cpp"
#include "FastRunningMedian.h"

#ifndef digitalPinToInterrupt
inline unsigned char  digitalPinToInterrupt(unsigned char Interrupt_pin) { return Interrupt_pin; } //This isn't included in the stm32duino libs (yet)
#endif
#ifndef portOutputRegister
#define portOutputRegister(port) (volatile byte *)( &(port->regs->ODR) ) //These are defined in STM32F1/variants/generic_stm32f103c/variant.h but return a non byte* value
#endif
#ifndef portInputRegister
#define portInputRegister(port) (volatile byte *)( &(port->regs->IDR) ) //These are defined in STM32F1/variants/generic_stm32f103c/variant.h but return a non byte* value
#endif

#define ENC_A 5
#define ENC_B 4

Ultrasound *ultrasound1;
int lastUs1Value;
FastRunningMedian<unsigned int,5,30> us1Median;
EngineController& engineController = EngineController::instance();

// const long BAUD_RATE = 115200;  // 9600 for debug
const long BAUD_RATE = 57600;  // 9600 for debug

const int US_TRIGGER_PIN = 13;
const int US_ECHO_PIN = 12;

void setup()
{
  delay(3000); // power-up safety delay
  Serial.begin(BAUD_RATE);
                                                                                                                                                                                                                                 
  ultrasound1 = new Ultrasound(US_TRIGGER_PIN, US_ECHO_PIN);
  ultrasound1->setup();  
  engineController.setup(); 

  // Setup the encoders
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_A), EngineController::isrEncoderA, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENC_B), EngineController::isrEncoderB, FALLING);

  initLedStrip();
}

void sendEvent(String name, int value)
{
  Serial.print(name);
  Serial.print(":");
  Serial.println(value);
  Serial.flush(); // Experimental
}

void loop()
{
  // Read sensors
  us1Median.addValue(ultrasound1->getDistance());
  int us1Distance = us1Median.getMedian();

  // Send events
  if (lastUs1Value != us1Distance) {
    lastUs1Value = us1Distance;
    sendEvent("US1", us1Distance);
  }

  // Commands
  char* command = engineController.readLine();

  // EngineController  
  if (command[0] != '\0') {    
    engineController.executeCommand(command);  
  } else {
    engineController.continueCommand();
  }

  // LedsController
  refreshLeds(command);
  
  // Free command
  free(command);
}

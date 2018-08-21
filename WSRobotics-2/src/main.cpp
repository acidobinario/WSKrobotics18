#include <Arduino.h>
#include <PRIZM.h>     // include PRIZM library
PRIZM prizm;           // instantiate a PRIZM object “prizm” so we can use its functions


//-------easy actions----------
void OpenClaw(){
  prizm.setServoPosition(2,170);
}

void CloseClaw(){
  prizm.setServoPosition(2,10);
}

void ArmUp(){
  prizm.setServoPosition(1, 10);
}

void ArmDown(){
  prizm.setServoPosition(1, 170);
}


//------end easy actions-------


void setup() { 
  prizm.prizmBegin();         // initialize the PRIZM controller
  prizm.setServoSpeed(2,50);
  //prizm.setServoPosition(2,0);
  // prizm.setMotorInvert(2,1);      // invert the direction of DC Motor 1
}

void loop() {
  
}


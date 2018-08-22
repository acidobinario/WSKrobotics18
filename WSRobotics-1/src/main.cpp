#include <Arduino.h>
#include <PRIZM.h> // include PRIZM library
#include <Wire.h>
 
PRIZM prizm;           // instantiate a PRIZM object “prizm” so we can use its functions


int S[] = {0,0,0,0}; //array of sensors
int tcount = 0;
String st = "";
byte command;
int leftTrigg = 180;
int rightTrigg = 130;
int ll;
int rr;
int payload[3];

void updateData(){
  S[1] = prizm.readLineSensor(5); //izq
  S[2] = prizm.readLineSensor(4); //der
  if(ll > leftTrigg){S[0] = 1;}
  else{S[0] = 0;}
  if(rr > rightTrigg){S[3] = 1;}
  else{S[0] = 0;}
}

//-------easy actions----------
//-------ARM------------------

void OpenClaw(){
  prizm.setServoPosition(1,170);
}

void CloseClaw(){
  prizm.setServoPosition(1,10);
}

void ArmUp(){
  prizm.setServoPosition(2, 40);
}

void ArmDown(){
  prizm.setServoPosition(2, 150);
}

void grabTheThing(){
  delay(3000);
  OpenClaw();
  ArmDown();
  delay(3000);
  CloseClaw();
  delay(1000);
  ArmUp();
  delay(1000);
  ArmDown();
  delay(2000);
  OpenClaw();
}
//-------END ARM---------------
//-------Motors----------------
void forward(){
  Serial.println("forward");
  prizm.setMotorPowers(15,15);
  //prizm.setRedLED(LOW);
}
void left(){
  Serial.println("left");
  prizm.setMotorPowers(7,15);
  //prizm.setRedLED(LOW);
}
void right(){
  Serial.println("right");
  prizm.setMotorPowers(15,7);
  //prizm.setRedLED(LOW);
}
void Tleft(){
  Serial.println("T left");
  prizm.setMotorPowers(0,15);
  // bool x = true;
  // while(x){
  //   if(S[0] == 0 && S[3] == 0){
  //     x = false;
  //   }
  // }
}
void Tright(){
  Serial.println("T right");
  prizm.setMotorPowers(15,0);
  //bool x = true;
  // while(x){
  //   if(S[0] == 0 && S[3] == 0){
  //     x = false;
  //   }
  // }
}
void backLeft(){
  prizm.setMotorPowers(-15,-8);
}
void backRight(){
  prizm.setMotorPowers(-8,-15);
}
void Stop(){
  prizm.setMotorPowers(0,0);
}
void backwards(){
  //TODO
}
//-------End Motors------------
//------end easy actions-------
//respect
void payRespects(){
  forward();
}

void receiveEvent(int howMany) {
  payload[0] = Wire.read(); //direction
  payload[1] = Wire.read(); //claw
  payload[2] = Wire.read(); //arm
/*
    0 -> forward x(>=900) y(300-600)
    1 -> backward x(<=100) y(300-600)
    2 -> forward left x(>=900) y(100-200)
    3 -> forward right x(>=900) y(750-930)
    4 -> left x(300-600) y(<=50)
    5 -> right x(300-600) y(>1000)
    6 -> backwards left
    7 -> backwards right
    8 -> stop x(400-750) y(300-600)
*/
  prizm.setServoPosition(2, payload[2]);

  if(payload[1] == 1){ //close claw
    CloseClaw();
  }
  else if(payload[1] == 0){
    OpenClaw();
  }

  if(payload[3] == 0){
    forward()
  }
  else if(payload[3] == 1){
    backwards();
  }
  else if(payload[3] == 2){
    left();
  }
  else if(payload[3] == 3){
    right();
  }
  else if(payload[3] == 4){
    Tleft();
  }
  else if(payload[3] == 5){
    Tright();
  }
  // else if(payload[3] == 6){

  // }
  // else if(payload[3] == 7){
    
  // }
  else if(payload[3] == 8){
    Stop();
  }
}

void setup() { 
  Wire.begin(666);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  prizm.PrizmBegin();         // initialize the PRIZM controller
  prizm.setServoSpeed(2,25);
  prizm.setServoPosition(2,45);
  prizm.setMotorInvert(2,1);      // invert the direction of DC Motor 1
  Serial.begin(9600);
  ArmUp();
  
  backwards();

}

void FollowLine(){
  updateData();
  if(S[0] == 0 && S[1] == 0 && S[2] == 0 && S[3] == 0 && st!="f" ){ //white on both
    st = "f";
    forward();
  }
  else if(S[0] == 0 && S[1] == 1 && S[2] == 0 && S[3] == 0 && st!="r"){ //go left
    st = "r";
    right();
  }
  else if(S[0] == 0 && S[1] == 0 && S[2] == 1 && S[3] == 0 && st!="l"){ //go right
    st = "l";
    left();
  }
  else if((S[1] == 1 && S[2] == 1) || (S[0] == 1 && S[3] == 1)){ //go right
    Tright();
  }
}

void hopeItWorksUwU(){

}
void loop() {
  //FollowLine();
  hopeItWorksUwU(); 
}


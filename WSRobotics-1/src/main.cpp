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
  //prizm.setMotorPowers(15,15);
  //prizm.setRedLED(LOW);
}
void left(){
  Serial.println("left");
  //prizm.setMotorPowers(7,15);
  //prizm.setRedLED(LOW);
}
void right(){
  Serial.println("right");
  //prizm.setMotorPowers(15,7);
  //prizm.setRedLED(LOW);
}
void Tleft(){
  Serial.println("T left");
  //prizm.setMotorPowers(0,15);
  bool x = true;
  while(x){
    if(S[0] == 0 && S[3] == 0){
      x = false;
    }
  }
}
void Tright(){
  Serial.println("T right");
  //prizm.setMotorPowers(15,0);
  bool x = true;
  while(x){
    if(S[0] == 0 && S[3] == 0){
      x = false;
    }
  }
}
void backLeft(){
  prizm.setMotorPowers(-15,-8);
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
  ll  = Wire.read();
  rr = Wire.read();    // receive byte as an integer
  Serial.println("left " + String(ll) + " right " + String(rr));         // print the integer
  // //F orward
  // //B ack
  // //L eft
  // //R ight
  // // G forleft
  // // I forright
  // //H bakcleft
  // //j backright
  // //s top
  // //0 - q arm
  // //front ON light W
  // //front OFF light w
  // //back lights ON U
  // //backlight OFF u

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


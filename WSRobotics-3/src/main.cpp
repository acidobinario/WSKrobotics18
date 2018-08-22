#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
int count = 0;
const byte address[6] = "satan";
int payload[3];
int claw = 0;
int clawState = 0;
int lastclawState = 1;
int clawCount=0;

/*
data structure [direction, claw, arm]

direction can be:
    0 -> forward x(>=900) y(300-600)
    1 -> backward x(<=100) y(300-600)
    2 -> forward left x(>=900) y(100-200)
    3 -> forward right x(>=900) y(750-930)
    4 -> left x(300-600) y(<=50)
    5 -> right x(300-600) y(>1000)
    6 -> backwards left
    7 -> backwards right
    8 -> stop x(400-750) y(300-600)

    

claw can be 1 for open and 0 for closed (button)
arm is the degree of the servo (pot)
*/

int getDirection(){
    int x = analogRead(A0);
    int y = analogRead(A1);
    //Serial.print(String(x) + " " + String(y));
    if(x>=900 && (y>=300 && y <= 600)){
        return 0;       //forward x(>=900) y(300-600)
    }
    else if(x<=100 && (y>=300 && y <= 600)){
        return 1;       //backward x(<=100) y(300-600)
    }
    else if(x>=900 && (y>=100 && y <= 200)){
        return 2;       //forward left x(>=900) y(100-200)
    }
    else if(x>=900 && (y>=750 && y<=930)){
        return 3;       //forward right x(>=900) y(750-930)
    }
    else if((x>=300 && x<=600) && y<=50 ){
        return 4;       //left x(300-600) y(<=50)
    }
    else if((x>=300 && x<=600) && y>=1000){
        return 5;       //right x(300-600) y(>1000)
    }
    else if((x>=400 && x<= 750) && (y>=300 && y <= 600)){
        return 8;       //stop x(400-750) y(300-600)
    }
    else{
        return 500;
    }
}

int getArm(){
    int degree = map(analogRead(A2), 0, 1023, 0, 180);
    Serial.print(" " + String(degree) + " ");
    return degree;
}

void ClawCount(){
    clawState = digitalRead(2);
    if (clawState != lastclawState) {
        if(clawState == LOW){
            clawCount++;
            claw = clawCount % 2;
            delay(50);
        }
    }
    lastclawState = clawState;
    Serial.println(claw);
}

void setup() {
    pinMode(2, INPUT_PULLUP);
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

void loop() {
    //Serial.print(getDirection());
    ClawCount();
    payload[0] = getDirection();
    payload[1] = claw;
    payload[2] = getArm();;
    //const char text[] = "Hello World";
    radio.write(&payload, sizeof(payload));
    count++;
    delay(10);
}
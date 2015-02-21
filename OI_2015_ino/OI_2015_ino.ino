/*
  Blink
 Turns on an LED on for one second, then off for one second, repeatedly.
 
 This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
#include "MegaJoy.h"

int led = 13;
int macroA = 2;
int macroB = 3;
int macroC = 4;
int macroD = 5;
int cancelButton = 6;
int elevA = 7;
int elevB = 8;
int elevC = 9;
int elevD = 10;
int elevE = 11;
int elevF = 12;
int elevG = 13;
int rollerIn = 14;
int rollerOut = 15;
int gripperOn = 16;
int gripperOff = 17;
int calHigh = A0;
int calLow = A1;
int motorA = A2;
int motorB = A3;
int potIn = A4;
int diskBrake = A5;


int acceptableError = 20;
int speedSlider = 0;
int sensorValue = 0;


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(macroA, INPUT_PULLUP); 
  pinMode(macroB, INPUT_PULLUP);  
  pinMode(macroC, INPUT_PULLUP);  
  pinMode(macroD, INPUT_PULLUP);
  pinMode(elevB, INPUT_PULLUP);
  pinMode(elevC, INPUT_PULLUP);
  pinMode(elevD, INPUT_PULLUP);
  pinMode(elevE, INPUT_PULLUP);
  pinMode(elevF, INPUT_PULLUP);
  pinMode(elevG, INPUT_PULLUP);
  pinMode(calHigh, INPUT_PULLUP);
  pinMode(calLow, INPUT_PULLUP);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(diskBrake, INPUT_PULLUP);
  setupMegaJoy();
}

// the loop routine runs over and over again forever:
void loop() {
  if(digitalRead(elevA) == HIGH) {
    moveToArea(995);
  }
  else if(digitalRead(elevB) == HIGH) {
    moveToArea(825);
  }
  else if(digitalRead(elevC) == HIGH) {
    moveToArea(654);
  }
  else if(digitalRead(elevD) == HIGH) {
    moveToArea(493);
  }
  else if(digitalRead(elevE) == HIGH) {
    moveToArea(337);
  }
  else if(digitalRead(elevF) == HIGH) {
    moveToArea(178);
  }
  else if(digitalRead(elevG) == HIGH) {
    moveToArea(25);
  }


  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);   
}

megaJoyControllerData_t getControllerData(void){

  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins



  controllerData.buttonArray[0] = !digitalRead(macroA); 
  controllerData.buttonArray[1] = !digitalRead(macroB); 
  controllerData.buttonArray[2] = !digitalRead(macroC);
  controllerData.buttonArray[3] = !digitalRead(macroD); 
  controllerData.buttonArray[4] = !digitalRead(cancelButton);
  controllerData.buttonArray[5] = !digitalRead(calHigh);
  controllerData.buttonArray[6] = !digitalRead(calLow);
  controllerData.buttonArray[7] = !digitalRead(diskBrake);
  controllerData.buttonArray[8] = !digitalRead(rollerIn);
  controllerData.buttonArray[9] = !digitalRead(rollerOut);
  controllerData.buttonArray[10] = !digitalRead(gripperOn);
  controllerData.buttonArray[11] = !digitalRead(gripperOff);
  
  controllerData.buttonArray[12] = !digitalRead(elevA); 
  controllerData.buttonArray[13] = !digitalRead(elevB); 
  controllerData.buttonArray[14] = !digitalRead(elevC);
  controllerData.buttonArray[15] = !digitalRead(elevD); 
  controllerData.buttonArray[16] = !digitalRead(elevE);
  controllerData.buttonArray[17] = !digitalRead(elevF);
  controllerData.buttonArray[18] = !digitalRead(elevG);
  /*   
   
   Key for which name corresponds to which button (NOT APPLICABLE)
   
   
   controllerData.triangleOn //macroA
   controllerData.circleOn //macroB
   controllerData.squareOn //macroC
   controllerData.crossOn //macroD
   controllerData.dpadUpOn //not used
   controllerData.dpadDownOn //not used
   controllerData.dpadLeftOn //not used
   controllerData.dpadRightOn //not used
   controllerData.l1On      //cancelButton
   controllerData.r1On      //calHigh
   controllerData.selectOn  //calLow
   controllerData.startOn   //diskBrake
   controllerData.l2On      //rollerIn
   controllerdata.r2On      //rollerOut
   controllerData.l3On      //gripperOn
   controllerData.r3On      //gripperOff
   controllerData.homeOn //button 13 */

  /*
  // Test here, the UnoJoy interface will not work
   // with this enabled
   for (int i = 2; i <= 10; i++){
   if(!digitalRead(i)){
   Serial.println("on:");
   Serial.print(i);
   }
   //    digitalWrite(i, HIGH);
   }*/

  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  

  //For 2015 we only have 1 analog axis, which
  //is the slider

  sensorValue = analogRead(potIn);

  controllerData.analogAxisArray[0] = sensorValue;



  //  Serial.println(analogRead(A0));

  // And return the data!
  return controllerData;
}

void moveToArea(int desiredCoord) {
  sensorValue = analogRead(potIn);
  while(sensorValue > (desiredCoord + acceptableError)) {
    sensorValue = analogRead(potIn);

    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
    delay(abs(speedSlider*(desiredCoord-sensorValue)));
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    /*Serial.print("Current pos: ");
    Serial.print(sensorValue);
    Serial.print(" Desired coordinate ");
    Serial.println(desiredCoord);
    Serial.println("moving up..."); */
  }
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, LOW);
  while(sensorValue < (desiredCoord - acceptableError)) {
    sensorValue = analogRead(potIn);

    digitalWrite(motorA, LOW);
    digitalWrite(motorB, HIGH);
    delay(abs(speedSlider*(desiredCoord-sensorValue)));
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    /* Serial.print("Current pos: ");
    Serial.print(sensorValue);
    Serial.print(" Desired coordinate ");
    Serial.println(desiredCoord);
    Serial.println("moving down..."); */
  }
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, LOW);
  Serial.println("Done");
}

float mapAsFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}




#include "MegaJoy.h"

//LED to confirm Arduino power and operation
int led = 13;

//Macro buttons to perform teleop automated routines
int macroA = 2;
int macroB = 3;
int macroC = 4;
int macroD = 5;
int macroE = 6;

//To cancel elevator position change or macro
int cancelButton = 7;

//Preset elevator positions
int elevA = 8;
int elevB = 9;
int elevC = 10;
int elevD = 11;
int elevE = 12;
int elevF = 13;
int elevG = 14;
int elevH = 15;

//Will be a 3 position momentary toggle switch used to control the intake
int rollerIn = 16;
int rollerOut = 17;

//A two position toggle to control gripper operation
int gripperOn = 18;

//Another three position momentary toggle, this time to calibrate the elevator
int calHigh = 19;
int calLow = 20;

//To control the motor on the slider
int motorA = 21;
int motorB = 22;

//Input from the potentiometer on the slider, A0 denotes analog input 0
int potIn = A0;

//A two position toggle to control disk brake operation 
int diskBrake = 23;

//To control accuracy of the slider proportional loop
int acceptableError = 20;

//To control the speed of the slider
int speedSlider = 0.02;

//Will hold position of the slider
int sensorValue = 0;



void setup() {   
  
  //sets up pins as outputs / inputs
  pinMode(led, OUTPUT);
  digitalWrite(LED, HIGH);
  
  pinMode(macroA, INPUT_PULLUP); 
  pinMode(macroB, INPUT_PULLUP);  
  pinMode(macroC, INPUT_PULLUP);  
  pinMode(macroD, INPUT_PULLUP);
  pinMode(macroE, INPUT_PULLUP);
  
  pinMode(cancelButton, INPUT_PULLUP);
  
  pinMode(elevB, INPUT_PULLUP);
  pinMode(elevC, INPUT_PULLUP);
  pinMode(elevD, INPUT_PULLUP);
  pinMode(elevE, INPUT_PULLUP);
  pinMode(elevF, INPUT_PULLUP);
  pinMode(elevG, INPUT_PULLUP);
  pinMode(elevH, INPUT_PULLUP);
  
  pinMode(rollerIn, INPUT_PULLUP);
  pinMode(rollerOut, INPUT_PULLUP);
  
  pinMode(gripperOn, INPUT_PULLUP);
  
  pinMode(calHigh, INPUT_PULLUP);
  pinMode(calLow, INPUT_PULLUP);
  
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  
  pinMode(diskBrake, INPUT_PULLUP);
  
  //General megajoy setup stuff
  setupMegaJoy();
}


void loop() {
  

  //Megajoy setup stuff, calls controller setup method
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
  ButtonPressToMove();
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


  //sets directinput pins to current OI button outputs
  controllerData.buttonArray[0] = !digitalRead(macroA); 
  controllerData.buttonArray[1] = !digitalRead(macroB); 
  controllerData.buttonArray[2] = !digitalRead(macroC);
  controllerData.buttonArray[3] = !digitalRead(macroD);
  controllerData.buttonArray[4] = !digitalRead(macroE);
  
  controllerData.buttonArray[5] = !digitalRead(cancelButton);
  controllerData.buttonArray[6] = !digitalRead(calHigh);
  controllerData.buttonArray[7] = !digitalRead(calLow);
  controllerData.buttonArray[8] = !digitalRead(diskBrake);
  controllerData.buttonArray[9] = !digitalRead(rollerIn);
  controllerData.buttonArray[10] = !digitalRead(rollerOut);
  controllerData.buttonArray[11] = !digitalRead(gripperOn);
  
  controllerData.buttonArray[12] = !digitalRead(elevA); 
  controllerData.buttonArray[13] = !digitalRead(elevB); 
  controllerData.buttonArray[14] = !digitalRead(elevC);
  controllerData.buttonArray[15] = !digitalRead(elevD); 
  controllerData.buttonArray[16] = !digitalRead(elevE);
  controllerData.buttonArray[17] = !digitalRead(elevF);
  controllerData.buttonArray[18] = !digitalRead(elevG);
  controllerData.buttonArray[19] = !digitalRead(elevH);
  
  
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


  //For 2015 we only have 1 analog axis, which
  //is the slider

  //sets sensorValue var to position of slider
  sensorValue = analogRead(potIn);
  
  //Sets analog axis equal to slider value
  controllerData.analogAxisArray[0] = sensorValue;

   // And return the data!
  return controllerData;


  
 
}

// will move elevator to correct position on button press
//Is after data return so button updates will be instantaneous
void ButtonPressToMove() {
  
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
}

void moveToArea(int desiredCoord) {
  //sets sensorVal var to position of slider
  sensorValue = analogRead(potIn);
  
  //This loop will move the slider UP to desired position
  //Will move the slider up in steps proportional to the difference between the current and desired position
  while(sensorValue > (desiredCoord + acceptableError)) {
    sensorValue = analogRead(potIn);
  
    //moves slider UP for time proportional to error
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
    delay(abs(speedSlider*(desiredCoord-sensorValue)));
    
    //turns both motor ouputs LOW. The motor outputs cannot both be HIGH at the same time as that will blow the H-Bridge transistors
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
   
  }
  
  //More safety stuff to make sure both outputs are not HIGH at the same time
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, LOW);
  
  
  //This loop will move the slider DOWN to desired position.
  //Will move the slider down in steps proportional to the difference between the current and desired position
  while(sensorValue < (desiredCoord - acceptableError)) {
    sensorValue = analogRead(potIn);

    //moves slider UP for time proportional to error
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, HIGH);
    delay(abs(speedSlider*(desiredCoord-sensorValue)));
    
    //turns both motor ouputs LOW. The motor outputs cannot both be HIGH at the same time as that will blow the H-Bridge transistors
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    
  }
  
  //More safety stuff to make sure both outputs are not HIGH at the same time
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, LOW);
  Serial.println("Done");
}


//copy of Arduino map function that will return a float instead of an int
float mapAsFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}




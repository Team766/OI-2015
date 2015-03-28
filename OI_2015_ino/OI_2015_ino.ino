#include "MegaJoy.h"

//LED to confirm Arduino power and operation
int led = 52;
int ledGround = 50;

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
int rollerBIn = 24;
int rollerBOut = 25;

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
int acceptableError = 30;

//To control the speed of the slider
int speedSlider = 0.02;

//Will hold position of the slider
int sensorValue = 0;

void setup() {

  //sets up pins as outputs / inputs
  pinMode(led, OUTPUT);
  pinMode(ledGround, OUTPUT);
  digitalWrite(led, HIGH);
  digitalWrite(ledGround, LOW);

  pinMode(macroA, INPUT_PULLUP);
  pinMode(macroB, INPUT_PULLUP);
  pinMode(macroC, INPUT_PULLUP);
  pinMode(macroD, INPUT_PULLUP);
  pinMode(macroE, INPUT_PULLUP);

  pinMode(cancelButton, INPUT_PULLUP);

  pinMode(elevA, INPUT_PULLUP);
  pinMode(elevB, INPUT_PULLUP);
  pinMode(elevC, INPUT_PULLUP);
  pinMode(elevD, INPUT_PULLUP);
  pinMode(elevE, INPUT_PULLUP);
  pinMode(elevF, INPUT_PULLUP);
  pinMode(elevG, INPUT_PULLUP);
  pinMode(elevH, INPUT_PULLUP);

  pinMode(rollerIn, INPUT_PULLUP);
  pinMode(rollerOut, INPUT_PULLUP);
  pinMode(rollerBIn, INPUT_PULLUP);
  pinMode(rollerBOut, INPUT_PULLUP);

  pinMode(gripperOn, INPUT_PULLUP);


  pinMode(calHigh, INPUT_PULLUP);
  pinMode(calLow, INPUT_PULLUP);

  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);

  pinMode(diskBrake, INPUT_PULLUP);

  //General megajoy setup stuff
  setupMegaJoy();

  delay(1000);
  digitalWrite(led, LOW);
}


void loop() {
  //Megajoy setup stuff, calls controller setup method
  megaJoyControllerData_t controllerData = getControllerData();
  controllerData.buttonArray[2] |= (HIGH << 7);
  setControllerData(controllerData);
  ButtonPressToMove();
}

megaJoyControllerData_t getControllerData(void) {

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

  //controllerData.buttonArray[0] = B10101010;
  //controllerData.buttonArray[1] = B10101010;
  controllerData.buttonArray[0] = (!digitalRead(macroA) << 0);
  controllerData.buttonArray[0] |= (!digitalRead(macroB) << 1);
  controllerData.buttonArray[0] |= (!digitalRead(macroC) << 2);
  controllerData.buttonArray[0] |= (!digitalRead(macroD) << 3);
  controllerData.buttonArray[0] |= (!digitalRead(macroE) << 4);

  controllerData.buttonArray[0] |= (!digitalRead(cancelButton) << 5);
  controllerData.buttonArray[0] |= (!digitalRead(calHigh) << 6);
  controllerData.buttonArray[0] |= (!digitalRead(calLow) << 7);
  controllerData.buttonArray[1] = (digitalRead(diskBrake) << 0);
  controllerData.buttonArray[1] |= (!digitalRead(rollerIn) << 1);
  controllerData.buttonArray[1] |= (!digitalRead(rollerOut) << 2);
  controllerData.buttonArray[1] |= (digitalRead(gripperOn) << 3);

  controllerData.buttonArray[1] |= (!digitalRead(elevA) << 5);
  controllerData.buttonArray[1] |= (!digitalRead(elevB) << 6);
  controllerData.buttonArray[1] |= (!digitalRead(elevC) << 7);
  controllerData.buttonArray[2] = (!digitalRead(elevD) << 0);
  controllerData.buttonArray[2] |= (!digitalRead(elevE) << 1);
  controllerData.buttonArray[2] |= (!digitalRead(elevF) << 2);
  controllerData.buttonArray[2] |= (!digitalRead(elevG) << 3);
  controllerData.buttonArray[2] |= (!digitalRead(elevH) << 4);
  controllerData.buttonArray[2] |= (!digitalRead(rollerBIn) << 5);
  controllerData.buttonArray[2] |= (!digitalRead(rollerBOut) << 6);




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
  //Megajoy looks for an 8 bit analog value but the analogRead() function returns a 10 bit value
  //Mapping sensorValue to a range of 0-255 will ensure an 8 bit value is returned
  controllerData.analogAxisArray[0] = map(sensorValue, 0, 1023, 0, 255);






  // And return the data!
  return controllerData;




}

// will move elevator to correct position on button press
//Is after data return so button updates will be instantaneous
void ButtonPressToMove() {

  if (digitalRead(elevA) != HIGH) {
    moveToArea(1);
  }
  else if (digitalRead(elevB) != HIGH) {
    moveToArea(2);
  }
  else if (digitalRead(elevC) != HIGH) {
    moveToArea(3);
  }
  else if (digitalRead(elevD) != HIGH) {
    moveToArea(4);
  }
  else if (digitalRead(elevE) != HIGH) {
    moveToArea(5);
  }
  else if (digitalRead(elevF) != HIGH) {
    moveToArea(6);
  }
  else if (digitalRead(elevG) != HIGH) {
    moveToArea(7);
  }
  else if (digitalRead(elevH != HIGH)) {
    //NOT USED
  }
}


//Uses old movement system, not as fast but confirmed working
void moveToArea(int desiredPos) {
  int desiredCoord = map(desiredPos, 1, 7, 50, 974);

  digitalWrite(led, HIGH);
  while ((sensorValue < (desiredCoord - acceptableError)) || (sensorValue > (desiredCoord + acceptableError))) {
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    sensorValue = analogRead(potIn);
    if (sensorValue < (desiredCoord - acceptableError)) {
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, HIGH);
      delay(2);
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, LOW);
    }
    megaJoyControllerData_t controllerData = getControllerData();
    setControllerData(controllerData);
    if (sensorValue > (desiredCoord + acceptableError)) {
      digitalWrite(motorA, HIGH);
      digitalWrite(motorB, LOW);
      delay(20);
      digitalWrite(motorA, LOW);
      digitalWrite(motorB, LOW);
    }
  }
  digitalWrite(led, LOW);
}



//copy of Arduino map function that will return a float instead of an int
float mapAsFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



# OI-2015
2015 OI

Contributors: Kaveh Pezeshki
kaveh@pezeshki.org

<strong>Overview:</strong>

The 2015 OI is described in the following documents (potentially out of date):

<p><a href="https://docs.google.com/document/d/1-BWv_VpO2KGuVvN3qlltaCp3rLe5aKPpKCXOMTD8Otc/edit?usp=sharing">OI Buttons</a></p>

<p><a href="https://docs.google.com/document/d/1AfbShbp5Lmn5j9dMG43Zpw4TVNy1rpCjC5DWKS2fBRI/edit?usp=sharing">OI Design</a></p>

The OI will control the elevator, intake arms, and well as a variety of preprogrammed routines.

<strong>Elevator:</strong>
The elevator position control is divided in two parts, a linear servo and a button array.
The linear servo will allow for fine control of the elevator position while the buttons will move the elevator to preset positions. 
In order to keep the slider in sync with the elevator when buttons are used, the slider will correct its position when a button is used. 

The sequence when a button is pressed:

1. Button pressed
2. Updated button states reported to host computer
3. Slider moves to correct position

While the slider is moving the slider axis will be set to 0.

The preferred method here would be for the axis values to be updated immediately after the button is pressed.

The elevator claw will be controlled by a two position toggle switch.

There will be a three position momentary toggle for slider calibration.

<strong>DO NOT EVER MAKE BOTH H-BRIDGE INPUTS HIGH: THIS WILL DESTROY THE H-BRIDGE AND COULD BLOW THE ARDUINO</strong>

<strong>Intake arms:</strong>

The intake arms are controlled by a three position momentary toggle switch. The roboRIO will interpret these commands and run an automated sequence that will either pull a tote in or push it out.

<strong>Macro buttons:</strong>

There will be four buttons used to initiate routines during the teleoperated period of a match. These will also be used to select autonomous routines.

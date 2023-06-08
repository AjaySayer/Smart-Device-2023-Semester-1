# Design Document

## Smart Device Theme

> Smart Wheelchair

The idea is to make a smart wheelchair, a masterclass vehicle for any individual in need. This wheelchair will have many great functions, such as an engine to drive around, an ejector seat (just in case) and a self-destruct mechanism; for good measure. Along with some nice lights so everyone can see your cool wheelchair zipping around.

Within the confines of this assignment, a DC motor will function as the engine, with a potentiometer controlling its speed, and a line sensor turning it on and off. Its current state (on/off) will also be represented using the green and red LED’s (red for off, green for on)
The servo will be emulating the ejector seat, which will be activated by the sonar when someone gets too close, or when the PIR detects movement, also turning on/off the yellow LED. In order to emulate the self-destruct mechanism, we will make the piezo beep a couple of times, activated by the crash sensor.
The SD card will also be logging all relevant events.

## Behaviours

1. The SD card will log all relevant events
2. When the ignition (Line sensor) is pressed, it will turn on/off the engine (DC motor) along with the engine on (Green LED) and engine off (Red LED) lights
3. The accelerator (Potentiometer) will control the speed of the engine (DC motor)
4. When the motion detection system (PIR) or distance detection system (Sonar) detects something, it will activate the ejector seat (Servo)
5. When the self destruct button is pressed (Crash sensor) the wheelchair will explode (Piezo buzzer)

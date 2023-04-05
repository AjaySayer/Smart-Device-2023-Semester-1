# Design Document

## Smart Device Theme

> Automatic intelligent car
The idea is to make an ‘intelligent car’ that will attempt to go as fast as possible to the desired gps coordinates, switching gears like a methed up redneck. The potentiometer can function as an accelerator or speed selector, with the DC motor acting as the engine. The servo can be used to indicate the changing of gears. The traffic lights can become activated based on the different pedals being pressed at any given time, with the sonar sensor working as a brake, because we’ve gotta slow down when people get close to the car. The line sensor can be used to start the vehicle, activating the piezo. It can also be activated by braking too hard or something. The crash sensor can be used as the handbrake, causing a crash when at high speeds using the piezo to showcase this. With the SD card logging the speed and current gear etc. 




## Behaviours

1. The SD card will log all events
2. The sonar will slow down the DC motor and affect the brightness of the red traffic light
3. The line sensor will start/stop the DC Motor and activate the Piezo
4. The crash sensor will activate the Piezo 
5. The Potentiometer will affect the speed of the DC motor and the brightness of the green traffic light, as well as the Servo through shifts
6. The GPS will be used to simulate a location the car will be driving too

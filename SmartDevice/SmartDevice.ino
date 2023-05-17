// SD Card Module
#include <SPI.h>
#include <SD.h>

// Real Time Clock (RTC)
#include "RTClib.h"
RTC_Millis rtc;     // Software Real Time Clock (RTC)
DateTime rightNow;  // used to store the current time.

// SD Card - Confirm Pin
#define SDpin 53

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Open serial communications and wait for port to open:
  while (!Serial) {
    delay(1);  // wait for serial port to connect. Needed for native USB port only
  }

  // SD Card initialisation
  Serial.print("Initializing SD card...");
  if (!SD.begin(SDpin)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  


  
  delay(250);
}

/*
 The user controls the speed of the engine (DC Motor) using the potentiometer (Accelerator)

 @params none
 @returns none
 */
 
void engineSpeed(){
  
}


/*
 The Sonar will activate the Ejector seat (Servo Motor)

 @params none
 @returns none
 */

void ejectorSeat() {

  
}


/*
 The line sensor will turn the engine (DC Motor) on or off

 @params none
 @returns none
 */

void engineStart() {

  
}


/*
 The GPS will track the wheelchair and use the traffic lights to do stuff

 @params none
 @returns none
 */

void trackingSystem() {

  
}


/*
 The self destruct button (Crash Sensor) will blow up the Vehicle (Piezo)

 @params none
 @returns none
 */

void selfDestruct() {

  
}

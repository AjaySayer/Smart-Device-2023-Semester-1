// SD Card Module
#include <SPI.h>
#include <SD.h>

// Real Time Clock (RTC)
#include "RTClib.h"
RTC_Millis rtc;     // Software Real Time Clock (RTC)
DateTime rightNow;  // used to store the current time.

// SD Card - Confirm Pin
#define SDpin 53

// DC Motor 
int E1 = 6;
int M1 = 5;

// Servo
#include <Servo.h>
Servo myservo;

// Potentiometer
#define pot A3

// Line Sensor
#define lineSensorPin 3

// Piezo Buzzer
#define piezoPin 8

// Traffic Lights - LED Outputs
#define ledRed A0
#define ledYellow A1 
#define ledGreen A2

// Sonar - HC-SR04
#define echoPin 6 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin A4 //attach pin D3 Arduino to pin Trig of HC-SR04

// Crash Sensor / Button
#define crashSensor 7

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Open serial communications and wait for port to open:
  while (!Serial) {
    delay(1);  // wait for serial port to connect. Needed for native USB port only
  }

 digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, HIGH);
  
  // SD Card initialisation
  Serial.print("Initializing SD card...");
  if (!SD.begin(SDpin)) {
    Serial.println("initialization failed!");
    while (1);
    
  }

  // Real Time Clock (RTC)
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("initialization done.");
  logEvent("System Initialisation...");

   // Traffic Lights - LED Outputs
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  
  // DC Motor
  pinMode(M1, OUTPUT);

  // Servo
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  //Potentiometer
  pinMode(pot, INPUT);

  // Piezo Buzzer
  pinMode(piezoPin,OUTPUT);

  // Sonar - HC-SR04
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  // Line Sensor
  pinMode(lineSensorPin, OUTPUT);

  // Crash Sensor / Button
  pinMode(crashSensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int potValue = analogRead(pot);            // reads the value of the potentiometer (value between 0 and 1023)

  Serial.print("potValue is: ");
  Serial.println(potValue);



  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, HIGH);


  //Serial.println("test");
  int speedValue = 255; // Can be 0-255.
  digitalWrite(M1,HIGH);
  analogWrite(E1, 255);   //PWM Speed Control

  //ejectorSeat();
  
  delay(250);
}

/*
  The user controls the speed of the engine (DC Motor) using the potentiometer (Accelerator)

  @params none
  @returns none
*/

void engineSpeed() {

}


/*
  The Sonar will activate the Ejector seat (Servo Motor)

  @params none
  @returns none
*/

void ejectorSeat() {
// Servo position values range from 0-180
  int servoPos = 0;
  myservo.write(servoPos);
  delay(5000);
  // Servo position values range from 0-180
  servoPos = 180;
  myservo.write(servoPos);
  delay(5000);
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

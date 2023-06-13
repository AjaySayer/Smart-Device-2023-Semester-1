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
#define M2 5  // attach pin D5 Arduino to pin M2 of DFROBOT Motor Controller
#define M2 5  // attach pin D5 Arduino to pin M2 of DFROBOT Motor Controller

// Servo
#include <Servo.h>
Servo myservo;

// Potentiometer
#define pot A3  // attach pin A3 Arduino to Potentiometer

// Line Sensor
#define lineSensorPin 3 // attach pin D3 Arduino to Line sensor

// Piezo Buzzer
#define piezoPin 8  // attach pin D8 Arduino to Piezo

// Traffic Lights - LED Outputs
#define ledRed A0 // attach pin A0 Arduino to Red Traffic Light
#define ledYellow A1  // attach pin A1 Arduino to Yellow Traffic Light
#define ledGreen A2 // attach pin A2 Arduino to Green Traffic Light

// Sonar - HC-SR04
#define echoPin 2   // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin A4  // attach pin A4 Arduino to pin Trig of HC-SR04

// Crash Sensor / Button
#define crashSensor 7 // attach pin D7 Arduino to Crash Sensor

// Passive Infrared Sensor
#define pirPin 4 // attach pin D4 Arduino to PIR

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
    while (1);
  }

  // Real Time Clock (RTC)
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("initialization done.");
  logEvent("System Initialisation...");

  // Traffic Lights - LED Outputs
  pinMode(ledRed, OUTPUT);  // Sets ledRed as an OUTPUT
  pinMode(ledYellow, OUTPUT); // Sets ledYellow as an OUTPUT
  pinMode(ledGreen, OUTPUT);  // Sets ledGreen as an OUTPUT

  // DC Motor
  pinMode(M2, OUTPUT);  // Sets M2 as an OUTPUT

  // Servo
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  //Potentiometer
  pinMode(pot, INPUT);  // Sets pot as an INPUT

  // Piezo Buzzer
  pinMode(piezoPin, OUTPUT);  // Sets the piezoPin as an OUTPUT

  // Sonar - HC-SR04
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);   // Sets the echoPin as an INPUT

  // Line Sensor
  pinMode(lineSensorPin, INPUT);  // Sets the lineSensorPin as an INPUT

  // Crash Sensor / Button
  pinMode(crashSensor, INPUT);  // Sets the crashSensor as an INPUT

  //PIR
  pinMode(pirPin, INPUT); // Sets the pirPin as an INPUT

  //Turns on all the LED's before the main code starts to make sure they work
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledGreen, HIGH);
}

void loop() {  
  // put your main code here, to run repeatedly:

  engineControl(engineIgnition(), engineSpeed());  // calls the engineControl, engineIgnition, and engineSpeed functions, with the latter two acting as parameters for the first
  ejectorSeat(detectionSystem()); // calls the ejectorSeat and detectionSystem function, with the ejectorSeat function acting as a parameter
  selfDestruct(); // calls the selfDestruct function

  delay(200); // adds 200ms of delay so the code doesnt run too fast
}
/*
  This function alternates the value of integer 'ignitionStatus' whilst the line sensor detects something. This essentially makes the line sensor act as a switch 
  assuming you quickly activate and deactivate it. Then the function returns the ignitionStatus integer which is then used to tell the engineControl function if the 
  engine should be on

  @params none
  @returns engineIgnition
*/
boolean engineIgnition() {
  static int ignitionStatus = 0;
  boolean lineSensorValue = digitalRead(lineSensorPin);
  Serial.print("lineSensorValue is: ");
  Serial.println(lineSensorValue);
  if (lineSensorValue == 1) {
    ignitionStatus++;
  }
  if (ignitionStatus > 1) {
    ignitionStatus = 0;
  }
  return ignitionStatus;
}
/*
  Takes input from the potentiometer and divides it by 4 so that the DC motor can function when more than 1/4th the
  potentiometer is being used
   
  @params none
  @returns speed
*/
int engineSpeed() {
  int potValue = analogRead(pot);
  Serial.print("potValue is: ");
  Serial.println(potValue);
  int speed = potValue / 4;  // Potentiometer inputs between 0-1024, DC motor only takes 0-256 so dividing the input by 4 keeps things simple
  Serial.print("Engine speed is: ");
  Serial.println(speed);
  return speed;
}
/*
  Using the input from the engineIgnition and engineSpeed functions (the line sensor and potentiometer) this function controls the engine (DC motor)
  turning it on/off and altering the speed based on the input from the functions

  @params engineIgnition, engineSpeed
  @returns none
*/
void engineControl(boolean engineIgnition, int engineSpeed) {
  if (engineIgnition == 1) {
    digitalWrite(M2, HIGH);
    analogWrite(E2, engineSpeed);  //PWM Speed Control
    Serial.print("Engine speed is: ");
    Serial.println(engineSpeed);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
  }

  if (engineIgnition == 0) {
    Serial.println("Engine OFF");
    digitalWrite(M2, HIGH);
    analogWrite(E2, 0);  //PWM Speed Control
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
  }
}
/*
  This function gets detects input from the Sonar sensor and PIR and outputs a return value of 1 if something is detected, and 0 if nothing is detected.

  @params none
  @returns 1, 0
*/
boolean detectionSystem() {
  long duration;
  int distance;
  int motion = digitalRead(pirPin);
  Serial.print("PIR value is: ");
  Serial.println(motion);
  // Generate a short pulse to trigger the sonar sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the time it takes for the sound wave to return
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;

  if (distance < 20 || motion == 1) {
    return 1;
  } else {
    return 0;
  }
}
/*
  This function takes input from the detectionSystem function and uses it to activate the ejector seat(servo) and turn on/off the yellow traffic light.
  when something is detected in less than 20cm (more like 7 in real life) this function will activate the servo, swinging it in the opposite direction
  and turn on the yellow traffic light. When nothing is detected the yellow traffic light turns off and the servo goes back to its default position.

  @params detection
  @returns none
*/
void ejectorSeat(boolean detection) {
  static int servoPos = 0;
  if (detection == 1) {
    Serial.println("Something detected!");
    digitalWrite(ledYellow, HIGH);
    servoPos = 180;  // Servo position values range from 0-180
    myservo.write(servoPos);
  } else {
    Serial.println("Nothing detected!");
    digitalWrite(ledYellow, LOW);
    servoPos = 0;
    myservo.write(servoPos);
  }
}
/*
  This function takes the input from the button and uses it to play 2 short beeps on the peizo when pressed

  @params none
  @returns none
*/
void selfDestruct() {
  bool isPressed = digitalRead(crashSensor);
  if (isPressed == LOW) {
    for (int i = 0; i < 2; i++) {
      tone(piezoPin, 1000, 200);  // Play a tone at 1000 Hz for 200 milliseconds
      delay(300);                 // Wait for 300 milliseconds between beeps
    }
  }
}
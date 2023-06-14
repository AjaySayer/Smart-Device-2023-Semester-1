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
#define E2 6  // attach pin D5 Arduino to pin E2 of DFROBOT Motor Controller

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

  delay(250); // adds 250ms of delay so the code doesnt run too fast
}
/*
  This function alternates the value of integer 'ignitionStatus' whilst the line sensor detects something. This essentially makes the line sensor act as a switch 
  assuming you quickly activate and deactivate it. Then the function returns the ignitionStatus integer which is then used to tell the engineControl function if the 
  engine should be on

  @params none
  @returns engineIgnition
*/
boolean engineIgnition() {
  /* A static int is an integer that is only created once, despite being inside of a function that runs in a loop. If this wasnt a static int "igntionStatus"
  would constantly be reset to 0, breaking the intended function of this function*/
  static int ignitionStatus = 0;
  boolean lineSensorValue = digitalRead(lineSensorPin);
  /* All Serial.print(); and Serial.println(); statements should be replaced by logEvent(); However when the code tried to log something to 
  the SD card, the code would freeze and cause everything to stop working. So Serial.print was used instead*/
  Serial.print("lineSensorValue is: ");
  Serial.println(lineSensorValue);
  /* These two if statements make a loop that will increase the value of ignitionStatus whilst the Line sensor detects something, but sets it back to 0 once the value
  of ignitionStatus goes over 1. this means that ignitionStatus only has two possible values, on(1)and off(0), which is what we need for our engine.*/
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
  potentiometer is being used. It is also possible to use "map" to do this, but I figured this is simpler than that.
   
  @params none
  @returns speed
*/
int engineSpeed() {
  int potValue = analogRead(pot);
  //Serial.print("potValue is: "); <-- this information is no longer useful, commented out
  //Serial.println(potValue);
  int speed = potValue / 4;  // Potentiometer inputs between 0-1024, DC motor can only an input of 0-255 so dividing the input by 4 keeps things simple
  //Serial.print("Engine speed is: "); <--- commented this out as it may be redundant due to the engineControl function printing the same thing, just never noticed
  //Serial.println(speed);
  return speed;
}
/*
  Using the input from the engineIgnition and engineSpeed functions (the Line Sensor and Potentiometer) this function controls the engine (DC motor)
  turning it on/off and altering the speed based on the input from the functions. It also turns on/off the Green and Red traffic lights accordingly

  @params engineIgnition, engineSpeed
  @returns none
*/
void engineControl(boolean engineIgnition, int engineSpeed) {
  if (engineIgnition == 1) { // if the engine is on
    digitalWrite(M2, HIGH);
    analogWrite(E2, engineSpeed);  // control the speed using the potentiometer
    Serial.print("Engine speed is: "); // print the speed of the engine
    Serial.println(engineSpeed);
    digitalWrite(ledRed, LOW); // turn off the red traffic light
    digitalWrite(ledGreen, HIGH); // turn on the green traffic light
  }

  if (engineIgnition == 0) { // if the engine is off
    Serial.println("Engine OFF"); // print that the engine is off
    digitalWrite(M2, HIGH);
    analogWrite(E2, 0); // set the speed to 0
    digitalWrite(ledRed, HIGH); // turn on the red traffic light
    digitalWrite(ledGreen, LOW); // turn off the green traffic light
  }
}
/*
  This function detects input from the Sonar sensor and the PIR and outputs a return value of 1 if something is detected, and 0 if nothing is detected.
  unfortunately the PIR can be too sensitive and detect what seems to be nothing, meaning that the detectionSystem function is stuck returning 1, keeping the 
  servo fully activated and preventing the Sonar from doing its job. This function may also be responsible for a strange bug that causes the servo to randomly
  activate/deactivate for a few hundred milliseconds every now and again.

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

  //if either the PIR or Sonar detects something return 1, otherwise return 0
  if (distance < 20 || motion == 1) {
    return 1;
  } else {
    return 0;
  }
}
/*
  This function takes input from the detectionSystem function and uses it to activate the ejector seat(servo) and turn on/off the yellow traffic light.
  when something is detected in less than 20cm (more like 7 in real life) by the sonar, or the PIR detects any movement, the detectionSystem function
  will return 1. That will cause this function to activate the servo, swinging it in the opposite direction and turn on the yellow traffic light. 
  When nothing is detected the yellow traffic light turns off and the servo goes back to its default position.

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
  This function takes the input from the crash sensor and uses it to play 2 short beeps on the peizo when pressed

  @params none
  @returns none
*/
void selfDestruct() {
  bool isPressed = digitalRead(crashSensor);
  if (isPressed == LOW) {
    for (int i = 0; i < 2; i++) {
      tone(piezoPin, 1000, 200);  // Plays a tone at 1000 Hz for 200 milliseconds
      delay(300);                 // Wait for 300 milliseconds between beeps
    }
  }
}

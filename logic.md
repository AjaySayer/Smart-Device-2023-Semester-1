# Programming Logic
## Engine Control <br>- Ignition (Line Sensor, Red & Green LED) <br>- Accelerator (Potentiometer)
```mermaid
 flowchart TD
    potInput([Potentiometer Input])
    lineSensorInput([Line Sensor Input])

    lineSensorInput-->lineSensorDecision
    lineSensorDecision{Does the line sensor detect something?}
    -->|Yes|switchFunction
    lineSensorDecision-->|No|complete
    switchFunction(Alternate boolean switch function<br> value)
    switchFunction-->switchDecision

    switchDecision{What is the boolean <br>switch function value?}
    -->|1/TRUE|engineStart
    engineStart("- Start DC motor<br>- Turn on Green LED<br>- Turn off Red LED")
    -->engineSpeedDecision

    switchDecision-->|0/FALSE|engineStop
    engineStop("- Stop DC motor<br>- Turn off Green LED<br>- Turn on Red LED")

    engineStart-->complete
    engineStop-->complete

    potInput-->potDecision
    potDecision{Has the potentiometer input <br>value changed?}
    -->|Yes|setSpeed
    potDecision-->|No|complete

    setSpeed(Take potentiometer input value and divide by 4)
    -->engineSpeedDecision
    engineSpeedDecision{Does the boolean switch <br>function value = 1/TRUE?}
    -->|Yes|engineSpeed
    engineSpeedDecision-->|No|complete
    engineSpeed(Set DC motor speed)
    -->complete

    complete([End])
```
## Ejector Seat (Sonar, PIR, Servo & Yellow LED)
```mermaid 
flowchart TD
    sonarInput([Sonar Input])
    PIRInput([PIR Input])

    sonarInput-->Decision
    PIRInput-->Decision

    Decision{Has something been detected?}

    Decision-->|Yes|on

    on(Activate the Servo & <br> turn on the yellow LED)
    on-->complete

    Decision-->|No|off
    off(Turn off the yellow LED)
    off-->complete


    complete([End])
```
## Self Destruct (Crash sensor & Piezo)
 ```mermaid
 flowchart TD
    crashSensorInput([Crash Sensor Input])

    crashSensorInput-->crashSensorDecision

    crashSensorDecision{Has the Crash Sensor<br/>Been Pressed?}

    crashSensorDecision-->|Yes|crashSensor

    crashSensor(Make Piezo beep twice)
    crashSensor-->complete

    crashSensorDecision-->|No|complete

    complete([End])
```
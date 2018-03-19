###### See on [GitHub](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/docs/AudioInfo.md) or download an [app]().

## Audio communication
This app can be interlocked with an external feeder connected to the other devices.<br>
The connection method using the most inexpensive and simple method as long as the creators come up with adopted, is to reduce as much as possible the burden on the researchers. The method is the one used to sound communication, but the presentation of sound stimulation by the use of the speed limit and sound communication of it process itself also to future updates in its influence is limited. Please note that.<br>

<br>

## Connect with Arduino

#### About

A sample method using Arduino® is as follows.<br>
1. Output the sound signal from earphone terminal of an iOS device.
2. The sound wave Arduino® takes as an analog value. When the Arduino® detects a certain level or more of the sound wave, electrify of 5V-DC to the specified PIN.
3. This treatment is realized the operation of the feeder by sound communication.

#### A sample code

The sample code of receiving audio signal in Arduino®, copy the following code and paste it into input area of the Arduino® IDE software. After that, select \"Upload to I/O Board\" from the \"File menu\".
```java
int val=0;
byte Feeder = 13;

void setup() {
    // Serial.begin(9800);
    pinMode(Feeder, OUTPUT);
}

void loop() {
    val=analogRead(0);
    // Serial.println(val);

    if(val >= 25){
        digitalWrite(Feeder,HIGH);
        delay(100);
        digitalWrite (Feeder,LOW);
    }
}
```
###### You can get the sample code on [here](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/codes/Arduino/AudioCommunication/AudioCommunication.ino).

#### A circuit diagram

![cd1](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/cd1.png "cd1")

#### Procedure

1. To prepare, an iOS device (you have installed this app), an Arduino (written the code), and an earphone cord.
![p1](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p1.png "p1")

2. Remove the film on the side of the tip is not a terminal mono earphone cord, divide the lines in two.
![p2](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p2.png "p2")

	2.1. Optionally, finished by using a heat-shrinkable tube and jumpers wire.
	![p3](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p3.png "p3")

3. Insert one of the earphone cord to GND pin of Arduino®, and the other end to A0 pin.
![p4](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p4.png "p4")

4. Insert one of the lines of the feeder (or line leading to it) to the GND pin, and the other end to the D13 pin (please be careful if there is a polarity). 
![p5](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p5.png "p5")

5. Android™ to connect the earphone, connect Arduino® to power, Android® app start with \"ExperimentType: Experiment\". 
![p6](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p6.png "p6")

6. Reinforcement of the app, is a success if feeder is operated by associated! 
![p7](https://raw.githubusercontent.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/pics/ArduinoWithSound/p7.png "p7")
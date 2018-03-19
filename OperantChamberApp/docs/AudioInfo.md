###### See on [GitHub](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/docs/AudioInfo.md) or download an [app]().

## Audio communication
This app can be interlocked with an external feeder connected to the other devices.<br>
The connection method using the most inexpensive and simple method as long as the creators come up with adopted, is to reduce as much as possible the burden on the researchers. The method is the one used to sound communication, but the presentation of sound stimulation by the use of the speed limit and sound communication of it process itself also to future updates in its influence is limited. Please note that.

<br>

## Connect with Arduino

#### About

A sample method using Arduino® is as follows.<br>
1. Output the sound signal from earphone terminal of an iOS device.
2. The sound wave Arduino® takes as an analog value. When the Arduino® detects a certain level or more of the sound wave, electrify of 5V-DC to the specified PIN.
3. This treatment is realized the operation of the feeder by sound communication.

<br>

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

<br>

#### A circuit diagram

![cd1](https://user-images.githubusercontent.com/22558921/37591062-9de2cf4c-2bad-11e8-86c9-ab2d66c32d69.png)

<br>

#### Procedure

1. To prepare, an iOS device (you have installed this app), an Arduino (written the code), and an earphone cord.
![p1](https://user-images.githubusercontent.com/22558921/37591113-c5510fd0-2bad-11e8-899e-0102bd5650a9.png)

2. Remove the film on the side of the tip is not a terminal mono earphone cord, divide the lines in two.
![p2](https://user-images.githubusercontent.com/22558921/37591114-c57e053a-2bad-11e8-9d0e-a53dc04f5b87.png)

	2.1. Optionally, finished by using a heat-shrinkable tube and jumpers wire.
	![p3](https://user-images.githubusercontent.com/22558921/37591115-c5a9b540-2bad-11e8-8366-ae77594f0479.png)

3. Insert one of the earphone cord to GND pin of Arduino®, and the other end to A0 pin.
![p4](https://user-images.githubusercontent.com/22558921/37591116-c5d6803e-2bad-11e8-8c4f-895d7306dfac.png)

4. Insert one of the lines of the feeder (or line leading to it) to the GND pin, and the other end to the D13 pin (please be careful if there is a polarity). 
![p5](https://user-images.githubusercontent.com/22558921/37591117-c60161dc-2bad-11e8-935c-dfa032dd6b68.png)

5. The iOS device to connect the earphone, connect Arduino® to power, the app start with turning on "Use SR sound" option. 
![p6](https://user-images.githubusercontent.com/22558921/37591118-c62d7eca-2bad-11e8-8799-8318fb7a9cab.png)

6. Reinforcement of the app, is a success if feeder is operated by associated! 
![p7](https://user-images.githubusercontent.com/22558921/37591119-c66a2758-2bad-11e8-9297-502c4e6921b5.png)
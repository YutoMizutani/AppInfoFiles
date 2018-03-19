## Audio communication
This app can be interlocked with an external feeder connected to the other devices.<br>
The connection method using the most inexpensive and simple method as long as the creators come up with adopted, is to reduce as much as possible the burden on the researchers. The method is the one used to sound communication, but the presentation of sound stimulation by the use of the speed limit and sound communication of it process itself also to future updates in its influence is limited. Please note that.<br>

<br>

### Connect with Arduino
A sample method using Arduino®︎ is as follows.<br>
1. Output the sound signal from earphone terminal of an iOS device.
2. The sound wave Arduino® takes as an analog value. When the Arduino® detects a certain level or more of the sound wave, electrify of 5V-DC to the specified PIN.
3. This treatment is realized the operation of the feeder by sound communication.

#### A sample code
The sample code of receiving audio signal in Arduino®, copy the following code and paste it into input area of the Arduino® IDE software. After that, select \"Upload to I/O Board\" from the \"File menu\".
```java
int val=0;\nbyte Feeder = 13;

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

By removing \"//\" portion of the code, it is possible to check the input values on the PC.


1. To prepare, an iOS device (you have installed this app), an Arduino (written the code), and an earphone cord.
![ss1](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/ArduinoWithSound/p1.png "p1")

2. Remove the film on the side of the tip is not a terminal mono earphone cord, divide the lines in two.
![ss2](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/ArduinoWithSound/p2.png "p2")

2.1. Optionally, finished by using a heat-shrinkable tube and jumpers wire.
![ss3](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/ArduinoWithSound/p3.png "p3")

3. Insert one of the earphone cord to GND pin of Arduino®, and the other end to A0 pin.


Blazing fast Markdown rendering in Swift, built upon [cmark](https://github.com/jgm/cmark).

Is your app using it? [Let us know!](mailto:rob@robphillips.me)

#### Maintainers

- [Rob Phillips](https://github.com/iwasrobbed)
- [Keaton Burleson](https://github.com/128keaton)
- [Other contributors](https://github.com/iwasrobbed/Down/graphs/contributors) 🙌

### Installation

Note: Swift 4 support is now on the `master` branch and any tag >= 0.4.x (Swift 3 is 0.3.x)

Quickly install using [CocoaPods](https://cocoapods.org):

```ruby
pod 'Down'
```

Or [Carthage](https://github.com/Carthage/Carthage):

```
github "iwasrobbed/Down"
```
Due to limitations in Carthage regarding platform specification, you need to define the platform with Carthage.

e.g.

````carthage update --platform iOS```

Or manually install:

1. Clone this repository
2. Build the Down project
3. Add the resulting framework file to your project
4. ?
5. Profit

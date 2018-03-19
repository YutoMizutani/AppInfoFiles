###### See on [GitHub](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/docs/AudioInfo.md) or download an [app]().

## Bluetooth LE communication
This app can be connected to the other devices using bluetooth low technology.<br>
Bluetooth LE can connect without authorization, so peripheral devices without knowing when established connection. It is easy and suitable for experiment.

<br>

## Peripheral mode (in app)

#### About

The app supported BLE both of central mode and peripheral mode.
A schedule of reinforcement parent is central, peripheral mode is only signal send to central.

#### Central mode

1. Select "Select session"
2. Select experiment parameters
3. Check "Use BLE Central mode"
4. After start, select connecting devices.

![central](https://user-images.githubusercontent.com/22558921/37593377-1d5e8c14-2bb5-11e8-9399-4c88489f9159.jpg)

#### Peripheral mode

1. Select "Peripheral mode"
2. Select apparatus and start! (Auto enabled BLE)

![peripheral](https://user-images.githubusercontent.com/22558921/37593378-1d8ac838-2bb5-11e8-9bf4-690132cee21f.jpg)

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
#include <SoftwareSerial.h>

/*-----------------------------------------------------------------*/

SoftwareSerial BTserial(2, 3); // RN4020-TX->Arduino-PIN2-RX, RN4020-RX->Arduino-PIN3-TX
const int lenEMB = 7; // (0 to 7):max length of event marker.
//[lenEMB変数の範囲について]
//使わないと分かっているならばそのbyte数だけ切り捨てることで処理速度が向上します。
//Arduino UNOではInt型が2byte()
int response = 0;

/*=================================================================*/

unsigned long IntbyteToUL(int x, int y) { //0xXY -> uLong
  // Serial.print("Debug-IntbyteToUL:");Serial.println(x * 16 + y);
  return x * 16 + y;
}

int HexadecimalToInt(char chr) {//e.g. 'A' -> 10
  switch (chr) {
    case 'A':
      return 10;
    case 'B':
      return 11;
    case 'C':
      return 12;
    case 'D':
      return 13;
    case 'E':
      return 14;
    case 'F':
      return 15;
    default:
      return chr - '0';
  }
}
unsigned long StrLittleEndianToUL(String str) { //
  unsigned long result = 0;
  for (int i = 0; i <= lenEMB; i++) {
    unsigned long digit = (i == 0 ? 1 : pow(256, i) + 1);
    //Serial.println("Debug-i:"+String(i));
    //Serial.print("Debug-digit:");Serial.println(digit);
    //Serial.println("Debug-UL:"+String(str[i * 2])+", "+String(str[i * 2+1]));
    result += IntbyteToUL(HexadecimalToInt(str[i * 2]), HexadecimalToInt(str[i * 2 + 1])) * digit;
    //Serial.print("Debug-result:");Serial.println(result);
  }
  return result;
}

/*=================================================================*/

void setup() {
  Serial.begin(9600);
  reset();
}
void reset() {
  BTserial.begin(115200);
  Serial.println("Initializing.");
  delay(100);
  BTserial.write("SF,1\r"); // reset configurations
  SendSerial();
  BTserial.write("SB,1\r"); // baudrate9600
  SendSerial();
  BTserial.write("R,1\r"); // reboot
  SendSerial();
  BTserial.end();
  delay(100);
  Serial.println("Initializing..");
  BTserial.begin(9600);
  delay(100);
  BTserial.write("+\r"); //Echo On
  SendSerial();
  Serial.println("Initializing...");
  Serial.println("--------------------------------");
  SendSerial();
  BTserial.write("SB,1\r"); //set baudrate 9600
  SendSerial();
  BTserial.write("SR,20006000\r"); //set peripheral, for iOS, server
  SendSerial();
  BTserial.write("SS,80000001\r"); //user-defined profile
  SendSerial();
  BTserial.write("ST,0010,0002,0064\r");// I:20ms,L:2,T:1s
  // interval>=16&&latency<=4&&timeout<=600
  // &&(interval + 16) * (latency + 1) < timeout * 8 / 3
  SendSerial();
  BTserial.write("PZ\r"); //clear PS & PC
  SendSerial();
  BTserial.write("PS,21BE11AB10F140CAA0CAA11A11515000\r"); //set private service
  SendSerial();
  //declare char
  BTserial.write("PC,21BE11AB10F140CAA0CAA11A11515001,1A,01\r"); //set private characteristics.
  BTserial.write("PC,21BE11AB10F140CAA0CAA11A11515002,1A,01\r"); //set private characteristics.
  SendSerial();
  BTserial.write("R,1\r");
  SendSerial();
  delay(1000);
  Serial.println("--------------------------------");
  Serial.println("completed!");
  Serial.println("--------------------------------");
}
/*-----------------------------------------------------------------*/
void SendSerial() {
  BTserial.flush();
  if (BTserial.available()) {
    String str = BTserial.readStringUntil('\n');
    Serial.println(str);
  }
  Serial.flush();
}

void loop() {
  if (Serial.available()) {
    //Command is '\r' or '\n'
    String str = Serial.readStringUntil('\r');
      String strx = str + '\r';
    if (str == "reset") {
      Serial.println("// Reset called!");
      reset();
    }else{
      int len = strx.length() + 1;
      char buf[len];
      strx.toCharArray(buf, len);
      BTserial.write(buf);
      Serial.println("//Serial read!:");
      Serial.println(buf);
      Serial.println("--------------------------------");
    }
  }
  if (BTserial.available()) {
    String str = BTserial.readStringUntil('\n');
    Serial.println("//BT read!:");
    //Serial.println(str.length());
    if (str.length() == 26) {
      //Serial.println("Debug-read:"+str);
      String strLittleEndian = str.substring(8, 24);
      //Serial.println("Debug-strLE:" + strLittleEndian);
      unsigned long readNum = StrLittleEndianToUL(strLittleEndian);
      Serial.println("Num:" + String(readNum));
      UserFunc(readNum);
    } else {
      Serial.println(str);
    }
    Serial.println("--------------------------------");
  }
  Serial.flush();
}

/*=================================================================*/

void UserFunc(unsigned long num) {
  switch (num) {
    //Write your own some functions corresponding the Event Marker.
    case 0: //EM:00
      //Serial.println("test");
      break;
    case 2:
      digitalWrite(12, HIGH);
      Serial.println("ON!");
      break;
    case 3:
      digitalWrite(12, LOW);
      Serial.println("OFF!");
      break;
    default:
      break;
  }
}

```
###### You can get the sample code on [here](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/codes/Arduino/BLECommunication/BLECommunication.ino).

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

5. The iOS device to connect the earphone, connect Arduino® to power, the app start with turning on \"Use SR sound\" option. 
![p6](https://user-images.githubusercontent.com/22558921/37591118-c62d7eca-2bad-11e8-8799-8318fb7a9cab.png)

6. Reinforcement of the app, is a success if feeder is operated by associated! 
![p7](https://user-images.githubusercontent.com/22558921/37591119-c66a2758-2bad-11e8-9297-502c4e6921b5.png)
###### See on [GitHub](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/docs/AudioInfo.md) or download an [app](https://itunes.apple.com/jp/app/operantchamberapp/id1250835517).

## Bluetooth LE communication

This app can be connected to the other devices using bluetooth low technology.<br>
Bluetooth LE can connect without authorization, so peripheral devices without knowing when established connection. It is easy and suitable for experiment.

<br>

## Algorithm

![al1](https://user-images.githubusercontent.com/22558921/37642638-40e222de-2c61-11e8-9592-146ff95fd071.png)

![al2](https://user-images.githubusercontent.com/22558921/37642639-41156dec-2c61-11e8-892f-e6740789488b.png)

This app use number of bytes send/receive data for Bluetooth LE. The method is faster than translate data from text or number.

1 byte is 8 bits, it has the range 0-255. Number of bytes Integer (Int64) in Swift has 8 bytes. The range of Int64 is -2,147,483,648 to 2,147,483,647. Event marker used experiment has not necessary to use too many numbers. This algorithm to change number of number of bits. For example, sending 100 formatted from Integer, the bytes are 8, but sending 100 formatted from Data (byte array), the byte is 1. This method is faster than normally sending.

<br>

## Central/Peripheral mode (in app)

#### About

The app supported BLE both of central mode and peripheral mode.
A schedule of reinforcement parent is central, peripheral mode is only signal send to central.

<br>

#### Central mode

1. Select "Select session"
2. Select experiment parameters
3. Check "Use BLE Central mode"
4. After start, select connecting devices.

![central](https://user-images.githubusercontent.com/22558921/37593377-1d5e8c14-2bb5-11e8-9399-4c88489f9159.jpg)

<br>

#### Peripheral mode

1. Select "Peripheral mode"
2. Select apparatus and start! (Auto enabled BLE)

![peripheral](https://user-images.githubusercontent.com/22558921/37593378-1d8ac838-2bb5-11e8-9bf4-690132cee21f.jpg)

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

###### You can get the sample code on [here](https://github.com/YutoMizutani/AppInfoFiles/blob/master/OperantChamberApp/src/codes/Arduino/BLECommunication/BLECommunication.ino).
```java
/*-----------------------------------------------------------------*/
 
#include <SoftwareSerial.h>

/*-----------------------------------------------------------------*/

SoftwareSerial BTserial(2, 3); // RN4020-TX->Arduino-PIN2-RX, RN4020-RX->Arduino-PIN3-TX

int response = 0;

/*=================================================================*/

unsigned long stringToUnsignedLong(String x) {
  unsigned long long y = 0;

  for (int i = 0; i < x.length(); i++) {
      char c = x.charAt(i);
     if (c < '0' || c > '9') break;
     y *= 10;
     y += (c - '0');
  }
  
  return y;
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
      Serial.println("// Serial read!:");
      Serial.println(buf);
      Serial.println("--------------------------------");
    }
  }
  if (BTserial.available()) {
    String str = BTserial.readStringUntil('\n');
    Serial.println("// BT read!:");
    int len = str.length();
    if (len >= 12) {
      int range = len - 10;
      String numString = str.substring(8, 8+range);
      unsigned long num = stringToUnsignedLong(numString);
      UserFunc(num);
    } else {
      Serial.println(str);
    }
    Serial.println("--------------------------------");
  }
  Serial.flush();
}

/*=================================================================*/

void UserFunc(unsigned long num) {
  Serial.println("Input: " + String(num));
  switch (num) {
    //Write your own some functions corresponding the Event Marker.
    case 1:
      response++;
      break;
    case 3:
      digitalWrite(13, HIGH);
      Serial.println("SRON!");
      break;
    case 4:
      digitalWrite(13, LOW);
      Serial.println("SROFF!");
      break;
    case 7:
      Serial.println("--------------------------------");
      Serial.println("Session finished!");
      Serial.println("Total responses: " + String(response));
      Serial.println("--------------------------------");
      reset();
      break;
    default:
      break;
  }
}

```

<br>

#### Circuit diagram

![cd1](https://user-images.githubusercontent.com/22558921/37642556-dd7b59fe-2c60-11e8-96d4-fdcd624a1a42.png)

![rn4020](https://user-images.githubusercontent.com/22558921/37634953-2a5fc3ce-2c3c-11e8-8c6e-38401823077a.jpeg)

<br>
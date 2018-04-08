/*=================================================================
  [BLECommunicaion.ino]
  A serial connection program using BLE for iOS.
  * ArduinoをRN4020のBLEによってiOSと通信するプログラムです。
  * Arduino UNOのSerial通信のBufferサイズは16Byteです。シリアルモニタ等で出力を確認する場合，初期設定時のEcho commandが文字化け（取りこぼし）します。送信には影響しません。
  * Arduinoが受け取り可能な値は0-4294967295(=32bit)です。iOS送信値は0-9223372036854775807(=64bit/2)です。
  * Arduino側の改行設定は"Carriage return"に指定してください。
  
  Copyright (c) 2018 Yuto Mizutani.
  This software released under MIT lisence.
 =================================================================*/
 
#include <SoftwareSerial.h>

/*-----------------------------------------------------------------*/

SoftwareSerial BTserial(2, 3); // RN4020-TX->Arduino-PIN2-RX, RN4020-RX->Arduino-PIN3-TX

// PIN numbers
const int lever = 7;
const int leverLED = 8;
const int feeder = 13;

// Parameters:
//    - currentState: State of lever pushing. 0: false, 1: true
//    - response: Number of response received from BLE
int currentState = 0;
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
  pinMode(lever, INPUT_PULLUP);
  pinMode(leverState, OUTPUT);
  pinMode(feeder, OUTPUT);
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
  // BTserial.write("PC,21BE11AB10F140CAA0CAA11A11515002,1A,01\r"); //set private characteristics.
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
/*-----------------------------------------------------------------*/
void leverState() {
  if (currentState != digitalRead(lever)) {
    currentState = digitalRead(lever);
    digitalWrite(leverLED, !currentState);
    sendResponse(currentState);
  }
}
void sendResponse(int state) {
  if (state) {
    Serial.println(state);
    BTserial.write("SHW,0018,01\r");
  }
}
/*-----------------------------------------------------------------*/

void loop() {
  leverState();
  if (Serial.available()) {
    // Command is '\r' or '\n'
    String str = Serial.readStringUntil('\r');
      String strx = str + '\r';
    if (str == "reset") {
      Serial.println("// Reset called!");
      reset();
    }else{
      int len = strx.length() + 1;
      char buf[len];
      strx.toCharArray(buf, len);
      Serial.println("// Serial read!:");
      Serial.println(buf);
      Serial.println("--------------------------------");
      BTserial.write(buf);
    }
  }
  if (BTserial.available()) {
    String str = BTserial.readStringUntil('\n');
    Serial.println("// BT read!:");
    int len = str.length();
    if (len >= 12) {
      Serial.print(str);
      Serial.print(' ');
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
  delay(1);
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
      digitalWrite(feeder, HIGH);
      Serial.println("SRON!");
      break;
    case 4:
      digitalWrite(feeder, LOW);
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

/*=================================================================*/

/*
MIT License

Copyright (c) 2018 Yuto Mizutani

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


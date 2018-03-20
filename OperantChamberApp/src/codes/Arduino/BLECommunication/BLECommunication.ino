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
const int lenEMB = 7;//(0 to 7):max length of event marker.
//[lenEMB変数の範囲について]
//使わないと分かっているならばそのbyte数だけ切り捨てることで処理速度が向上します。
//Arduino UNOではInt型が2byte()
int response = 0;

/*=================================================================*/

unsigned long IntbyteToUL(int x, int y) { //0xXY -> uLong
  //Serial.print("Debug-IntbyteToUL:");Serial.println(x * 16 + y);
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
      digitalWrite(13, HIGH);
      Serial.println("ON!");
      break;
    case 3:
      digitalWrite(13, LOW);
      Serial.println("OFF!");
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


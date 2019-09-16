/* HID Joystick Mouse Example
   by: Jim Lindblom
   date: 1/12/2012
   license: MIT License - Feel free to use this code for any purpose.
   No restrictions. Just keep this license if you go on to use this
   code in your future endeavors! Reuse and share.
 
   This is very simplistic code that allows you to turn the 
   SparkFun Thumb Joystick (http://www.sparkfun.com/products/9032)
   into an HID Mouse. The select button on the joystick is set up
   as the mouse left click. 
 */

#include "Mouse.h"
#include <ArduinoJson.h>

#include <SoftwareSerial.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
char inChar;
int buttonState = 0;
SoftwareSerial mySerial(8, 9); // RX, TX
StaticJsonDocument<200> doc;
int yaw_1, pitch_1, roll_1;

void setup()
{
  Serial.begin(115200);
  mySerial.begin(57600);
  pinMode(3, INPUT);  // Set both analog pins as inputs
  pinMode(4, INPUT);
  delay(1000);  // short delay to let outputs settle
}

void loop()
{
  buttonState = digitalRead(3);
  if (mySerial.available()) {
    // get the new byte:
    inChar = (char)mySerial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '}') {
      stringComplete = true;
    }
  }
 if(stringComplete)
 {
    //char data[200];
   // inputString.toCharArray(data, 200);
    DeserializationError error = deserializeJson(doc,inputString );
    //delay(100);  // short delay to let outputs settle
    yaw_1 = doc["yaw"];
    pitch_1= doc["pitch"];
    roll_1 = doc["roll"];
    Serial.println(roll_1);
    inputString = "";
    stringComplete = 0;
    Mouse.move(map(pitch_1,-200,200,0,800), map(roll_1,-200,200,0,800), 0);  // move mouse on y axis
 }
//  if (buttonState!= 0)
//  {
//    Mouse.move(map(pitch_1,-200,200,0,800), map(roll_1,-200,200,0,800), 0);  // move mouse on y axis
//  } 
////  if ((digitalRead(selPin) == 0) && (!mouseClickFlag))  // if the joystick button is pressed
//  {
//    mouseClickFlag = 1;
//    Mouse.press(MOUSE_LEFT);  // click the left button down
//  }
//  else if ((digitalRead(selPin))&&(mouseClickFlag)) // if the joystick button is not pressed
//  {
//    mouseClickFlag = 0;
//    Mouse.release(MOUSE_LEFT);  // release the left button
//  }
}

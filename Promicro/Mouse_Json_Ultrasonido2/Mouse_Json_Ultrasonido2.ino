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
#include <Ultrasonic.h>

Ultrasonic ultrasonic(3, 2);

#define sensitivity 10
#define switch_uno 7


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
char inChar;
SoftwareSerial mySerial(8, 9); // RX, TX
StaticJsonDocument<200> doc;
int yaw_1, pitch_1, roll_1;
int deltax, deltay;
boolean  mouse_enabled;
boolean Switch;
long duration;
int distance;
long time_ultrasound;
boolean distancia;
boolean medido;


void setup()
{
  Serial.begin(115200);
  mySerial.begin(57600);
  pinMode(switch_uno, INPUT);  // Set both analog pins as inputs
  pinMode(LED_BUILTIN, OUTPUT);
  ultrasonic.setTimeout(40000UL);
  time_ultrasound = millis();
  delay(1000);  // short delay to let outputs settle
  distancia = true;
}

void loop()
{
  Switch = digitalRead(switch_uno);
  if((distancia) && (!mouse_enabled))
  {
    if(millis()- time_ultrasound > 800)
     {
      Serial.print("Distance in CM: ");
      Serial.println(ultrasonic.read());
      time_ultrasound = millis();
     } 
  }

     if ((Switch== true) && (mouse_enabled == false))
  {
      Mouse.begin();
      mouse_enabled = true;
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(100);  
       
  } 
   if ((Switch== false) && (mouse_enabled == true))
   {
    Mouse.end();
    mouse_enabled = false;
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);   
    
   }
 if(mouse_enabled)
 {
      if (mySerial.available()) 
      {
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
       if((stringComplete) && (mouse_enabled))
      {
          //char data[200];
         // inputString.toCharArray(data, 200);
          DeserializationError error = deserializeJson(doc,inputString );
          //delay(100);  // short delay to let outputs settle
          yaw_1 = doc["yaw"];
          pitch_1= doc["pitch"];
          roll_1 = doc["roll"];
          inputString = "";
          stringComplete = 0;
          deltax = (pitch_1/sensitivity);
          deltay = (roll_1/sensitivity);
          Mouse.move(-deltax,-deltay, 0);  // move mouse on y axis
       }
 }

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

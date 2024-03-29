/* 
 
 */

#include <Mouse.h>  // Use built-in Mouse library
#include <ArduinoJson.h>

#include <SoftwareSerial.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(3, 2);

#define sensitivity 10
#define switch_uno 7  // IF ON -> ACELEROMETER OFF -> Mouse_ACCEL OFF & SONAR ON
#define btn1 18
#define btn2 15
#define btn3 14
#define btn4 16
#define btn5 10
#define tiempo_rebote 1000

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
int altura;
int btn;
boolean boton1,boton2,boton3,boton4,boton5;
double bt1,bt2,bt3,bt4,bt5;
double ahora;
void setup()
{
  Serial.begin(115200);
  mySerial.begin(57600);
  pinMode(switch_uno, INPUT);  // 
  pinMode(btn1, INPUT);  // 
  pinMode(btn2, INPUT);  // 
  pinMode(btn3, INPUT);  // 
  pinMode(btn4, INPUT);  // 
  pinMode(btn5, INPUT);  // 
  
  pinMode(LED_BUILTIN, OUTPUT);
  ultrasonic.setTimeout(40000UL);
  time_ultrasound = millis();
  delay(1000);  // short delay to let outputs settle
  distancia = true;
  altura = 0;
  btn = 0;
}

void enviar_msg()
{
   String msg =("{\"altura\":"+String(altura)+"," + "\"btn\":" +String(btn)+"}"); //JSON!*/ 
   Serial.println(msg);
}

void loop()
{
  Switch = digitalRead(switch_uno);
  boton1 = digitalRead(btn1);
  boton2 = digitalRead(btn2);
  boton3 = digitalRead(btn3);
  boton4 = digitalRead(btn4);
  boton5 = digitalRead(btn5);
  btn = 0;
  ahora = millis();
  if((boton1) && ((ahora-bt1)>tiempo_rebote))
  {
      if(mouse_enabled)
     {
      Mouse.click();
     }
    btn = 1;
    enviar_msg();
    bt1 = ahora;
  }
  if((boton2) && ((ahora-bt2)>tiempo_rebote))
  {
    
   if(mouse_enabled)
     {
      Mouse.click(MOUSE_RIGHT);
     }
    btn = 2;
    enviar_msg();
    bt2 = ahora;
  }
    if((boton3) && ((ahora-bt3)>tiempo_rebote))
  {
    btn = 3;
    enviar_msg();
    bt3 = ahora;
  }
    if((boton4) && ((ahora-bt4)>tiempo_rebote))
  {
    btn = 4;
    enviar_msg();
    bt4 = ahora;
  }
    if((boton5) && ((ahora-bt5)>tiempo_rebote))
  {
    btn = 5;
    enviar_msg();
    bt5 = ahora;
  }  
  
 
  if((distancia) && (!mouse_enabled))
  {
    if(millis()- time_ultrasound > 800)
     {
      //Serial.print("Distance in CM: ");
      //Serial.println(ultrasonic.read());
     altura = ultrasonic.read();
     time_ultrasound = millis();
     enviar_msg();
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
    enviar_msg();
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

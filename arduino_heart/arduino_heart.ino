/*
 * Script to read the value of the magnetics switch (connected to GND and a digital pin)
 * and depending on the value turn on/off a servo and a RGB LED
 * 
 * Pin configuration for WeMos: https://wiki.wemos.cc/products:d1:d1_mini
 * 
 * Use 3x 1k ohm resistor for the RGB LED
 */

#include <Servo.h>

// MAGNETIC SWITCH
// For Arduino Uno: 4 
// For Wemos: D3 or D4 (using D4 will result in activating the built-in LED blinking)
const int switchSensor = D3; 
int switchState; // 0 close - 1 open
int prevSwitchState = -1;

// SERVO
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
int stepAmount = 5; // how many degrees to move the servo by

// RGB LED
int redPin = D8;
int greenPin = D7;
int bluePin = D6;

// If fading
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

void setup()
{
  pinMode(switchSensor, INPUT_PULLUP);
  
  myservo.attach(D0);  // attaches the servo on pin 9 to the servo object

  // RGB LED output pins
  pinMode(redPin, OUTPUT); // red
  pinMode(greenPin, OUTPUT); // green
  pinMode(bluePin, OUTPUT); // blue
  //digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
  
  Serial.begin(115200);
}

void doHeartbeat()
{
  for (pos = 10; pos <= 170; pos += stepAmount) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 170; pos >= 10; pos -= stepAmount) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void turnRedOn()
{
  digitalWrite(redPin, LOW);
}

void turnRedOff()
{
  digitalWrite(redPin, HIGH);
}

void doColoredHeartbeat() {
  digitalWrite(redPin, LOW);
  for (pos = 10; pos <= 90; pos += stepAmount) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(greenPin, LOW);
  for (pos = 90; pos <= 170; pos += stepAmount) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(bluePin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, HIGH);
  for (pos = 170; pos >= 90; pos -= stepAmount) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(bluePin, HIGH);
  digitalWrite(redPin, LOW);
  for (pos = 90; pos >= 10; pos -= stepAmount) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void turnLedsOff() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void doRedFade() {
  for (brightness = 0; brightness <= 1023; brightness += fadeAmount) {
    analogWrite(redPin, brightness);
    delay(10);
  }
  for (brightness = 1023; brightness >= 0; brightness -= fadeAmount) {
    analogWrite(redPin, brightness);
    delay(10);
  }
}

void doRedFadeHeartbeat() {

  for (pos = 10; pos <= 170; pos += stepAmount) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    brightness = map(pos, 10, 170, 270, 1023);
    analogWrite(redPin, brightness);    
    delay(15);
  }
  for (pos = 170; pos >= 10; pos -= stepAmount) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    brightness = map(pos, 10, 170, 270, 1023);
    analogWrite(redPin, brightness); 
    delay(15);
  }
  
}

void loop()
{
  switchState = digitalRead(switchSensor);

  if (prevSwitchState != switchState) {
    prevSwitchState = switchState;
    if (switchState == HIGH){
        Serial.println(1); // door is open
      }
      else{
        Serial.println(0); // door is closed
      }
  }

  if (switchState == HIGH) {
    //turnRedOn();
    //doHeartbeat();
    //doColoredHeartbeat();
    doRedFadeHeartbeat();
  }
  else {
    //turnRedOff();
    //turnLedsOff();
    analogWrite(redPin, 1023); 
  }
  
  
  delay(200);
}

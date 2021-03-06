/******************************************************************************
  Main.ino
  Bizzy Bee Firmware
  Kevin Loeffler and Justin Barnitz
  12/5/16

  Developed for Teensy LC
******************************************************************************/

// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>
#include <Wire.h>
#include "SparkFunISL29125.h"
#include <SoftwareSerial.h>
// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 4
#define BIN1 7
#define AIN2 5
#define BIN2 8
#define PWMA 3
#define PWMB 17
#define STBY 6
#define thresh 1000
#define mospeed 80

// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

bool go = false; //Determines whether the robot should go or not.

int left_IR = 15;
int center_IR = 14;
int right_IR = 16;
int leftspeed = mospeed;
int rightspeed = mospeed;
bool finished = false;
char redDir = 'a';
char blueDir = 'a';
char greenDir = 'a';

int bluetoothTx = 9;  // TX-O pin of bluetooth mate, RX on Teensy
int bluetoothRx = 10;  // RX-I pin of bluetooth mate, TX on Teensy

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

// Declare sensor object
SFE_ISL29125 RGB_sensor;

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps

  pinMode(left_IR, INPUT);
  pinMode(center_IR, INPUT);
  pinMode(right_IR, INPUT);

  //RGB Sensor
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
}


void loop()
{
  int leftread = analogRead(left_IR);
  int center = analogRead(center_IR);
  int rightread = analogRead(right_IR);
  //Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();

  Serial.print("Left: "); Serial.println(leftread, DEC);
  Serial.print("Center: "); Serial.println(center, DEC);
  Serial.print("Right: "); Serial.println(rightread, DEC);

  while (bluetooth.available()) // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    //Serial.print((char)bluetooth.read());
    char currentChar = bluetooth.read();
    int exclamations = 0;
    if (currentChar = 'i') {
      char nextChar = bluetooth.read();
      if (nextChar = 'r') {
        char dirChar = bluetooth.read();
        redDir = dirChar;
      }
      if (nextChar = 'g') {
        char dirChar = bluetooth.read();
        greenDir = dirChar;
      }
      if (nextChar = 'b') {
        char dirChar = bluetooth.read();
        blueDir = dirChar;
      }
    }
    else if (currentChar = '!') {
      exclamations++;
    }

    if (exclamations = 2) {
      go = true; //The bluetooth message has finished.
    }
  }

  if (go) {
    if (blue>3000){
      if(blueDir='a'){
        right(motor1, motor2, mospeed);
        delay(1);
      }
      if(blueDir='d'){
        left(motor1, motor2, mospeed);
        delay(1);
      }
    }
    if (green>3000){
      if(greenDir='a'){
        right(motor1, motor2, mospeed);
        delay(1);
      }
      if(greenDir='d'){
        left(motor1, motor2, mospeed);
        delay(1);
      }
    }
    if (red>3000){
      if(redDir='a'){
        right(motor1, motor2, mospeed);
        delay(1);
      }
      if(redDir='d'){
        left(motor1, motor2, mospeed);
        delay(1);
      }
    }
    if (center < thresh) {
      //leftspeed=-mospeed;
      //rightspeed=mospeed;
      forward(motor1, motor2, mospeed);
      delay(1);
    }
    else if (rightread < thresh) {
      //leftspeed= -(mospeed+50);
      //rightspeed= mospeed-50;
      left(motor1, motor2, mospeed);
      delay(1);
    }
    else if (leftread < thresh) {
      //leftspeed= -(mospeed-50);
      //rightspeed= mospeed+50;
      right(motor1, motor2, mospeed);
      delay(1);
    }

    if ((leftread < thresh) && (rightread < thresh) && (center < thresh)) {
      leftspeed = 0;
      rightspeed = 0;
      finished = true;
    }

    if (!finished) {
      //motor1.drive(leftspeed,10);
      //motor2.drive(rightspeed,10);
    }
    else
    {
      motor1.drive(0, 10);
      motor2.drive(0, 10);
    }

  }

}

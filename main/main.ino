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

int left_IR = 15;
int center_IR = 14;
int right_IR = 16;
int leftspeed = mospeed;
int rightspeed = mospeed;
bool finished = false;

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

void setup()
{
 // Initialize serial communication
  Serial.begin(115200);

  pinMode(left_IR, INPUT);
  pinMode(center_IR, INPUT);
  pinMode(right_IR, INPUT);
}


void loop()
{
   int leftread = analogRead(left_IR);
   int center = analogRead(center_IR);
   int rightread = analogRead(right_IR);
   Serial.print("Left: "); Serial.println(leftread,DEC);
   Serial.print("Center: "); Serial.println(center,DEC);
   Serial.print("Right: "); Serial.println(rightread,DEC);
   
   if(center<thresh){
    //leftspeed=-mospeed;
    //rightspeed=mospeed;
    forward(motor1, motor2, mospeed);
    delay(1);
   }
   else if(rightread<thresh){
    //leftspeed= -(mospeed+50);
    //rightspeed= mospeed-50;
    left(motor1, motor2, mospeed);
    delay(1);
   }
   else if(leftread<thresh){
    //leftspeed= -(mospeed-50);
    //rightspeed= mospeed+50;
    right(motor1, motor2, mospeed);
    delay(1);
   }

   if((leftread<thresh) && (rightread<thresh) && (center < thresh)){
    leftspeed=0;
    rightspeed=0;
    finished=true;
   }

   if(!finished){
     //motor1.drive(leftspeed,10);
     //motor2.drive(rightspeed,10);
   }
   else
   {
    motor1.drive(0,10);
    motor2.drive(0,10);
   }
   
   /*
   //Use of the forward function, which takes as arguements two motors
   //and optionally a speed.  If a negative number is used for speed
   //it will go backwards
   forward(motor1, motor2, 50);
   delay(1000);
   
   //Use of the back function, which takes as arguments two motors 
   //and optionally a speed.  Either a positive number or a negative
   //number for speed will cause it to go backwards
   back(motor1, motor2, -50);
   delay(1000);
     
   //Use of the left and right functions which take as arguements two
   //motors and a speed.  This function turns both motors to move in 
   //the appropriate direction.  For turning a single motor use drive.
   left(motor1, motor2, 100);
   delay(1000);
   right(motor1, motor2, 100);
   delay(1000);
   
   //Use of brake again.
   brake(motor1, motor2);
   delay(1000);
   */
   
}

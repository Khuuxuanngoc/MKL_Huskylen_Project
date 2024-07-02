/*
Author  : Andrea Lombardo
Site    : https://www.lombardoandrea.com
Source  : https://github.com/AndreaLombardo/L298N/

Based on Arduino Basic Fade example.

Speed range go from 0 to 255, default is 100.
Use setSpeed(speed) to change.

Sometimes at lower speed motors seems not running.
It's normal, may depends by motor and power supply.

Wiring schema in file "L298N - Schema_with_EN_pin.png"
*/

// Include the library
#include "L298N.h"

// Pin Motor_1
const unsigned int Motor1_A = 8;
const unsigned int Motor2_A = 9;
const unsigned int Motor_A_speech = 6;
// Pin Motor_2
const unsigned int Motor1B = 4;
const unsigned int Motor2B = 7;
const unsigned int Motor_B_speech = 5;
// Create one motor instance
L298N motor1(Motor_A_speech, Motor1_A, Motor2_A);

L298N motor2(Motor_B_speech, Motor1B, Motor2B);
int speedness = 0;
int speedAmount = 1;

void setup()
{
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial)
  {
    // do nothing
  }
}

void loop()
{
  // Apply faded speed
  motor1.setSpeed(speedness);

  // Tell motor A to go forward (may depend by your wiring)
  motor1.forward();

  motor2.setSpeed(speedness);

  // Tell motor A to go forward (may depend by your wiring)
  motor2.forward();

  // Alternative method:
  // motor.run(L298N::FORWARD);

  // print the motor status in the serial monitor
  // printSomeInfo();

  // Change the "speedness" for next time through the loop:
  // speedness = speedness + speedAmount;

  // Reverse the direction of the fading at the ends of the fade:
  // if (speedness <= 0 || speedness >= 255)
  // {
  //   speedAmount = -speedAmount;
  // }

  // Wait for 30 milliseconds to see the dimming effect
  delay(30);
}

/*
Print some informations in Serial Monitor
*/
void printSomeInfo()
{
  Serial.print("Motor is moving = ");
  Serial.print(motor1.isMoving() ? "YES" : "NO");
  Serial.print(" at speed = ");
  Serial.println(motor1.getSpeed());
}
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
#include <L298NX2.h>

#include "HUSKYLENS.h" //Import the required libraries
#include "SoftwareSerial.h"
#include "PIDLoop_huy.h"
// Pin Motor_1
const unsigned int Motor1_A = 8;
const unsigned int Motor2_A = 9;
const unsigned int Motor_A_speech = 6;
// Pin Motor_2
const unsigned int Motor1B = 4;
const unsigned int Motor2B = 7;
const unsigned int Motor_B_speech = 5;
// Create one motor instance
L298NX2 motors(Motor_A_speech, Motor1_A, Motor2_A, Motor_B_speech, Motor1B, Motor2B);
// L298N motor1(Motor_A_speech, Motor1_A, Motor2_A);

// L298N motor2(Motor_B_speech, Motor1B, Motor2B);

int motorSpeed = 150;  // The maximum motor speed
byte motorOffset = 5; // Factor to account for one side being more powerful
// int motorOffset = 40; // Factor to account for one side being more powerful
unsigned long previousMillis = 0;
const long interval = 1000;
// int turnSpeed = 200; // Amount to add to motor speed when turning
int leftSpeed = 0; // Variables to keep current left and right motor speeds
int rightSpeed = 0;

PIDLoop headingLoop(400, 1, 0, false);
// PIDLoop SET_Speed(motorSpeed);

HUSKYLENS huskylens; // Create a Huskeylens object
int ID1 = 1;         // We're tracking recognised object 1
int32_t error;
void printResult(HUSKYLENSResult result);

void setup()
{

  Serial.begin(115200); // Start serial communication
  // DEBUG_SERIAL_CMD_LN("tempKd");
  Wire.begin(); // Begin communication with the Huskeylens
  motors.setSpeedA(leftSpeed);
  motors.setSpeedB(rightSpeed);
  while (!huskylens.begin(Wire))
  {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
  huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING); // Switch the algorithm to line tracking.
                                                     // rightBack.setSpeed(0);                             // Set the motors to the motor speed, initially all 0
                                                     // rightFront.setSpeed(0);
                                                     // leftFront.setSpeed(0);
                                                     // leftBack.setSpeed(0);
  // motor1.forward();
  // motor2.forward();
  accelerate(); // Accelerate to the set motor speed
}

void loop()
{
  // motor1.forward();
  // motor2.forward();
  motors.forwardA();
  motors.forwardB();
  Serial_update_pid(&headingLoop);
  // motorSpeed = setSpeed(&headingLoop);
  // motorSpeed = headingLoop.setSpeed();
  if (!huskylens.request(ID1)) // If a connection is not established
  {
    // Serial.println(F("Check connection to Huskeylens"));
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (!huskylens.isLearned()) // If an object has not been learned
  {
    // Serial.println(F("No object has been learned"));
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (!huskylens.available()) // If there is no arrow being shown on the screen yet

  {
    // Serial.println(F("No arrow on the screen yet"));
    if (millis() - previousMillis >= 0)
    {

      leftSpeed = 0;
      rightSpeed = 0;
      // DEBUG_SERIAL_CMD_LN(motorSpeed);
    }
  }
  else // Once a line is detected and an arrow shown
  {

    previousMillis = millis();

    // ss_error();
      HUSKYLENSResult result = huskylens.read(); // Read and display the result
  printResult(result);
      error = (int32_t)result.xTarget - (int32_t)160;



// if (error>50)
// {
//       leftSpeed = 70; // Calculate the total left side speed
//     rightSpeed = 0; // Calculate the total right side speed
// }else if (error<50)
// {
//       leftSpeed = 0; // Calculate the total left side speed
//     rightSpeed = 70; // Calculate the total right side speed
// }else{




    // leftSpeed = headingLoop.m_command;   // Get the left side speed change
    // rightSpeed = -headingLoop.m_command; // Get the right side speed change

    // leftSpeed += motorSpeed;                  // Calculate the total left side speed
    // rightSpeed += (motorSpeed - motorOffset); // Calculate the total right side speed
    headingLoop.update(error); // Perform PID control on the error
    leftSpeed = headingLoop.m_command;   // Get the left side speed change
    rightSpeed = -headingLoop.m_command; // Get the right side speed change

    leftSpeed += motorSpeed; // Calculate the total left side speed
    // rightSpeed += (motorSpeed + motorOffset); // Calculate the total right side speed
    rightSpeed += motorSpeed; // Calculate the total right side speed
  }


  motors.setSpeedA(leftSpeed);
  motors.setSpeedB(rightSpeed);

}

void ss_error()
{
  HUSKYLENSResult result = huskylens.read(); // Read and display the result
  printResult(result);
  int error_1;
  int error_2;
  error_1 = (int32_t)result.xTarget - 160;

  error_2 = (int32_t)result.xOrigin - 160;
  // error = (int32_t)result.xTarget - (int32_t)160;
  // error =error_1;
  if ((error_1 == error_2) && (error_1 == 0))
  {
    error = 0;
  }
  else if ((error_1 == error_2) && (error_1 > 0))
  {
    error = error_1;
  }
  else if ((error_1 == error_2) && (error_1 < 0))
  {
    error = error_1;
  }
  else if ((error_1 < error_2) && (error_1 = 0))
  {
    error = error_2;
  }
  else if ((error_1 < error_2) && (error_1 > 0))
  {
    error = error_2;
  }
  else if ((error_1 < error_2) && (error_1 < 0))
  {
    error = error_1;
  }
  else if ((error_1 > error_2) && (error_1 = 0))
  {
    error = error_2;
  }
  else if ((error_1 > error_2) && (error_1 > 0))
  {
    error = error_1;
  }
  else if ((error_1 > error_2) && (error_1 < 0))
  {
    error = error_2;
  }

  // Serial.println("error:  ");
  // Serial.println(error);
}

void printResult(HUSKYLENSResult result) // Display the results on the serial monitor
{
  if (result.command == COMMAND_RETURN_BLOCK)
  {
    // Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  }
  else if (result.command == COMMAND_RETURN_ARROW)
  {
    // Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  }
  else
  {
    // Serial.println("Object unknown!");
  }
}
void accelerate() // Function to accelerate the motors from 0 to full speed
{
  for (int i = 0; i < motorSpeed; i++) // Loop from 0 to full speed
  {
    // left
    // motor1.setSpeed(i + motorOffset);
    // // right
    // motor2.setSpeed(i);
    motors.setSpeedA(i + motorOffset);
    motors.setSpeedB(i);
    delay(10);
  }
}
// void Serial_update_Speed()
// {
//   if (Serial.available() > 0)
//   {
//     String tempString = Serial.readStringUntil('\n');
//     int tempNum = tempString.toInt();

//     // Serial.println(tempNum);
//     if (tempString.indexOf("Sp") > -1)
//     {
//       motorSpeed = tempNum;

//       DEBUG_SERIAL_CMD_T(F("Speed: "));
//       DEBUG_SERIAL_CMD_LN(motorSpeed);
//     }
//   }
// }
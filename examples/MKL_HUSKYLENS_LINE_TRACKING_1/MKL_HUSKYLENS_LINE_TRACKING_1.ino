/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows how to play with line tracking.
 
 Created 2020-03-13
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "PIDLoop.h"
#include "DFMobile.h"

#include "Makerlabvn_SimpleMotor.h"

// #define PIN_ENA 6       
// #define PIN_IN1 9       //! D4
// #define PIN_IN2 8       //! D7
// #define PIN_IN3 7       //! D9
// #define PIN_IN4 4       //! D8
// #define PIN_ENB 5

#define PIN_ENA 5       
#define PIN_IN1 7       //! D4
#define PIN_IN2 4       //! D7
#define PIN_IN3 9       //! D9
#define PIN_IN4 8       //! D8
#define PIN_ENB 6


#define ZUMO_FAST        100 //100

// DFMobile Robot (7,6,4,5);     // initiate the Motor pin

// OBJECT INITIALIZATION
Makerlabvn_SimpleMotor Robot(PIN_ENA, PIN_IN1, PIN_IN2, PIN_IN3, PIN_IN4, PIN_ENB);

PIDLoop headingLoop(200, 0, 50, true);
HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
int ID1 = 1;
void printResult(HUSKYLENSResult result);



void setup() {
    Serial.begin(115200);
    // Robot.Direction (HIGH, LOW);  // initiate the positive direction  
    Robot.car_stop();
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING); //Switch the algorithm to line tracking.

    // Robot.car_run(100, 100);
    // delay(3000);

    // Robot.car_run(0, 100);
    // delay(3000);

    // Robot.car_run(0, 0);
    // delay(3000);
}
int left = 0, right = 0;

void loop() {
    int32_t error; 
    if (!huskylens.request(ID1)) {Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));left = 0; right = 0;}
    else if(!huskylens.isLearned()) {Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));left = 0; right = 0;}
    else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        HUSKYLENSResult result = huskylens.read();
        printResult(result);

        // Calculate the error:
        error = (int32_t)result.xTarget - (int32_t)160;

        // Perform PID algorithm.
        headingLoop.update(error);

        // separate heading into left and right wheel velocities.
        left = headingLoop.m_command;
        right = -headingLoop.m_command;

        left += ZUMO_FAST;
        right += ZUMO_FAST;
    }

    Serial.println(String()+left+","+right);
    // Robot.Speed (left,right);
    Robot.car_run(left, right);
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}
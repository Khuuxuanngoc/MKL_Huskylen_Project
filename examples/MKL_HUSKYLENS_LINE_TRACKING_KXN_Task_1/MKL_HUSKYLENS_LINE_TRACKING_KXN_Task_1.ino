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
#include "kxnTask_Husky_Tracking.h"

kxnTask_Husky_Tracking kxnTask_Husky_Tracking1;

void setup() {
    kxnTask_Husky_Tracking1.setup();
}


void loop() {
    // kxnTask_Husky_Tracking1.loop();
    kxnTaskManager.run(millis());
    // int32_t error; 
    // if (!huskylens.request(ID1)) {Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));left = 0; right = 0;}
    // else if(!huskylens.isLearned()) {Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));left = 0; right = 0;}
    // else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    // else
    // {
    //     HUSKYLENSResult result = huskylens.read();
    //     printResult(result);

    //     // Calculate the error:
    //     error = (int32_t)result.xTarget - (int32_t)160;

    //     // Perform PID algorithm.
    //     headingLoop.update(error);

    //     // separate heading into left and right wheel velocities.
    //     left = headingLoop.m_command;
    //     right = -headingLoop.m_command;

    //     left += ZUMO_FAST;
    //     right += ZUMO_FAST;
    // }

    // Serial.println(String()+left+","+right);
    // // Robot.Speed (left,right);
    // Robot.car_run(left, right);
}

// void printResult(HUSKYLENSResult result){
//     if (result.command == COMMAND_RETURN_BLOCK){
//         Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
//     }
//     else if (result.command == COMMAND_RETURN_ARROW){
//         Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
//     }
//     else{
//         Serial.println("Object unknown!");
//     }
// }
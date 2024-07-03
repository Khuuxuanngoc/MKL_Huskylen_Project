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
#include "OneButton.h"

#define PIN_INPUT 10
OneButton button(PIN_INPUT, true);

kxnTask_Husky_Tracking kxnTask_Husky_Tracking1;

void setup() {
    button.attachDoubleClick(doubleClick);
    button.attachClick(kxnClick);
    button.attachLongPressStop(kxnLongClick);
    kxnTask_Husky_Tracking1.setup();
}


void loop() {
    // kxnTask_Husky_Tracking1.loop();
    kxnTaskManager.run(millis());
    // keep watching the push button:
    button.tick();
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


void doubleClick()
{
  Serial.println("x2");
  kxnTask_Husky_Tracking1.startTrackLine();

} // doubleClick

void kxnClick()
{
  Serial.println("x1");
  // kxnTask_Husky_Tracking1.startTrackObject();

  kxnTask_Husky_Tracking1.stop();
} // Click

void kxnLongClick()
{
  Serial.println("x1");
  kxnTask_Husky_Tracking1.startTrackObject();
} // Long Click
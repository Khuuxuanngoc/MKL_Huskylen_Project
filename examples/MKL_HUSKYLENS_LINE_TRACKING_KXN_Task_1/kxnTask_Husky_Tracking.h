#ifndef kxnTask_Husky_Tracking__H
#define kxnTask_Husky_Tracking__H
#include "kxnTask.h"

#include "HUSKYLENS.h"
// #include "SoftwareSerial.h"
#include "PIDLoop.h"
#include "DFMobile.h"

#include "Makerlabvn_SimpleMotor.h"

#define PIN_ENA 5
#define PIN_IN1 7
#define PIN_IN2 4
#define PIN_IN3 9
#define PIN_IN4 8
#define PIN_ENB 6

#define ZUMO_FAST 100  //100

Makerlabvn_SimpleMotor Robot(PIN_ENA, PIN_IN1, PIN_IN2, PIN_IN3, PIN_IN4, PIN_ENB);

PIDLoop headingLoop(200, 0, 50, true);

DEFINE_TASK_STATE(kxnTask_Husky_Tracking){
  kxnTask_Husky_Tracking_INIT,
  kxnTask_Husky_Tracking_RUN,
  kxnTask_Husky_Tracking_STOP,
};

CREATE_TASK(kxnTask_Husky_Tracking)

HUSKYLENS huskylens;
// //HUSKYLENS green line >> SDA; blue line >> SCL
int ID1 = 1;
int left = 0, right = 0;
int kxnOffsetPos1 = 10;
int kxnOffsetPos2 = 40;
int kxnOffsetPos3 = 100;

float scale1 = 0.5;
float scale2 = 1;
float scale3 = 2.5;
float scale4 = 4.5;

int kxnSpeed_L = 90, kxnSpeed_R = 90;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Robot.car_stop();
  Wire.begin();
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
  // huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);  //Switch the algorithm to line tracking.

  // Robot.car_run(100, 100);
  // delay(3000);

  // Robot.car_run(0, 100);
  // delay(3000);

  // Robot.car_run(0, 0);
  // delay(3000);

  // Robot.car_run(-100, -100);
  // delay(3000);

  // Robot.car_run(0, 0);
  // delay(3000);

  this->start();
}

void GetDataThenTracking() {
  int32_t error;
  if (!huskylens.request(ID1)) {
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    left = 0;
    right = 0;
  } else if (!huskylens.isLearned()) {
    Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    left = 0;
    right = 0;
  } else if (!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else {
    HUSKYLENSResult result = huskylens.read();
    printResult(result);

    // Calculate the error:
    // error = (int32_t)result.xTarget - (int32_t)160;
    error = (int32_t)result.xTarget;

    // Perform PID algorithm.
    headingLoop.update(error);

    // separate heading into left and right wheel velocities.
    left = headingLoop.m_command;
    right = -headingLoop.m_command;

    left += ZUMO_FAST;
    right += ZUMO_FAST;
  }

  // Serial.println(String() + left + "," + right);

  trackingProcess(error);
  // Robot.Speed (left,right);
  // Robot.car_run(left, right);
}

// 0----------160----------320

void trackingProcess(int pos_pa){
  long tempPos1 = abs(pos_pa - 160);
  long tempSpeed_L = 0;
  long tempSpeed_R = 0;
  // long tempPos = map(tempPos1, 0, 160, 0, 200);
  long tempPos = 0;

  if((pos_pa >= (160 + kxnOffsetPos1)) && (pos_pa < (160 + kxnOffsetPos2))){   //right
    tempPos = tempPos1 * scale1;
    tempSpeed_L = kxnSpeed_L + tempPos;
    tempSpeed_R = kxnSpeed_R - tempPos;
  }else if((pos_pa >= (160 + kxnOffsetPos2)) && (pos_pa < (160 + kxnOffsetPos3))){   //right
    tempPos = tempPos1 * scale2;
    tempSpeed_L = kxnSpeed_L + tempPos;
    tempSpeed_R = kxnSpeed_R - tempPos;
  }else if(pos_pa >= (160 + kxnOffsetPos3)){   //right
    tempPos = tempPos1 * scale3;
    tempSpeed_L = kxnSpeed_L + tempPos;
    tempSpeed_R = kxnSpeed_R - tempPos;
  }else if((pos_pa <= (160 - kxnOffsetPos1)) && (pos_pa > (160 - kxnOffsetPos2))){ // left
    tempPos = tempPos1 * scale1;
    tempSpeed_L = kxnSpeed_L - tempPos;
    tempSpeed_R = kxnSpeed_R + tempPos;
    // Robot.car_run(kxnSpeed_L - tempPos, kxnSpeed_R + tempPos);
  }else if((pos_pa <= (160 - kxnOffsetPos2)) && (pos_pa > (160 - kxnOffsetPos3))){ // left
    tempPos = tempPos1 * scale2;
    tempSpeed_L = kxnSpeed_L - tempPos;
    tempSpeed_R = kxnSpeed_R + tempPos;
    // Robot.car_run(kxnSpeed_L - tempPos, kxnSpeed_R + tempPos);
  }else if(pos_pa <= (160 - kxnOffsetPos3)){ // left
    tempPos = tempPos1 * scale3;
    tempSpeed_L = kxnSpeed_L - tempPos;
    tempSpeed_R = kxnSpeed_R + tempPos;
    // Robot.car_run(kxnSpeed_L - tempPos, kxnSpeed_R + tempPos);
  }else{
    tempSpeed_L = kxnSpeed_L;
    tempSpeed_R = kxnSpeed_R;
    // Robot.car_run(kxnSpeed_L, kxnSpeed_R);
  }
  Serial.println(String() + tempSpeed_L + "\t" + tempSpeed_R + "\t" + tempPos + "\t" + tempPos1);
  Robot.car_run(tempSpeed_L, tempSpeed_R);
}

void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  } else if (result.command == COMMAND_RETURN_ARROW) {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  } else {
    Serial.println("Object unknown!");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (getState()) {
    case kxnTask_Husky_Tracking_INIT:
      huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);  //Switch the algorithm to line tracking.
      

      setState(kxnTask_Husky_Tracking_RUN);
      kDelay(3000);
      break;

    case kxnTask_Husky_Tracking_RUN:
      GetDataThenTracking();
      kDelay(0);
      break;
  }
}

void start() {
  setState(kxnTask_Husky_Tracking_INIT);
  kxnTaskManager.add(this);
}

void stop() {
  kDelay(0);
  setStateIdle();
}

END

#endif
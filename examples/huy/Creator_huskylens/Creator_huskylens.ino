// #include "Creator_motor.h"
#define Motor_A1 8
#define Motor_A2 9
#define Motor_A_speech 6

#define Motor_B1 4
#define Motor_B2 7
#define Motor_B_speech 5
// Creator_motor motor;
void setup()
{
    pinMode(Motor_A1, OUTPUT);
    pinMode(Motor_A2, OUTPUT);
    pinMode(Motor_A_speech, OUTPUT);

    pinMode(Motor_B1, OUTPUT);
    pinMode(Motor_B2, OUTPUT);
    pinMode(Motor_B_speech, OUTPUT);
    delay(5000);
}
void loop()
{
    digitalWrite(Motor_A1, LOW);
    digitalWrite(Motor_A2, HIGH);

    analogWrite(Motor_A_speech, 200);

    // digitalWrite(Motor_B1, LOW);
    // digitalWrite(Motor_B2, HIGH);
    // analogWrite(Motor_B_speech, 200);
}
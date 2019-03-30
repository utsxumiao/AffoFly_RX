#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "Output.h"

#ifdef DC_MOTOR_DIFFERENTIAL_PWM

#define PWM_LEFT_PIN_1    5
#define PWM_LEFT_PIN_2    6
#define PWM_RIGHT_PIN_1   9
#define PWM_RIGHT_PIN_2   10

#define RC_MIN            1000
#define RC_MAX            2000
#define DDRIVE_MIN        -100
#define DDRIVE_MAX        100
#define MOTOR_MIN_PWM     -255
#define MOTOR_MAX_PWM     255

int16_t LeftMotorOutput;
int16_t RightMotorOutput;

void Output_init() {
  pinMode(PWM_LEFT_PIN_1, OUTPUT);
  pinMode(PWM_LEFT_PIN_2, OUTPUT);
  pinMode(PWM_RIGHT_PIN_1, OUTPUT);
  pinMode(PWM_RIGHT_PIN_2, OUTPUT);
  analogWrite(PWM_LEFT_PIN_1, 0);
  analogWrite(PWM_LEFT_PIN_2, 0);
  analogWrite(PWM_RIGHT_PIN_1, 0);
  analogWrite(PWM_RIGHT_PIN_2, 0);
}

void Output_data(ControlData controlData) {
  float rawLeft;
  float rawRight;

  float x = map(controlData.Roll, RC_MIN, RC_MAX, DDRIVE_MIN, DDRIVE_MAX);
  float y = map(controlData.Pitch, RC_MIN, RC_MAX, DDRIVE_MIN, DDRIVE_MAX);

  float z = sqrt(x * x + y * y);
  float rad = acos(abs(x) / z);
  if (isnan(rad) == true) {
    rad = 0;
  }
  float angle = rad * 180 / PI;
  float tcoeff = -1 + (angle / 90) * 2;
  float turn = tcoeff * abs(abs(y) - abs(x));
  turn = round(turn * 100) / 100;
  float mov = max(abs(y), abs(x));
  if ((x >= 0 && y >= 0) || (x < 0 && y < 0)) {
    rawLeft = mov; rawRight = turn;
  } else {
    rawRight = mov; rawLeft = turn;
  }
  if (y < 0) {
    rawLeft = 0 - rawLeft;
    rawRight = 0 - rawRight;
  }
  LeftMotorOutput = map(rawLeft, DDRIVE_MIN, DDRIVE_MAX, MOTOR_MIN_PWM, MOTOR_MAX_PWM);
  RightMotorOutput = map(rawRight, DDRIVE_MIN, DDRIVE_MAX, MOTOR_MIN_PWM, MOTOR_MAX_PWM);

#ifdef DC_MOTOR_SPEED_AUX
  float speedRatio;
  switch (DC_MOTOR_SPEED_AUX) {
    case 1:
      speedRatio = controlData.Aux1 / RC_MAX;
      break;
    case 2:
      speedRatio = controlData.Aux2 / RC_MAX;
      break;
    case 3:
      speedRatio = controlData.Aux3 / RC_MAX;
      break;
    case 4:
      speedRatio = controlData.Aux4 / RC_MAX;
      break;
    case 5:
      speedRatio = controlData.Aux5 / RC_MAX;
      break;
    case 6:
      speedRatio = controlData.Aux6 / RC_MAX;
      break;
    default:
      speedRatio = 1;
      break;
  }
  LeftMotorOutput *= speedRatio;
  RightMotorOutput *= speedRatio;
#endif

#ifdef DEBUG
  Serial.print("x: ");     Serial.print(x);           Serial.print("  ");
  Serial.print("y: ");     Serial.print(y);           Serial.print("  ");
  Serial.print("rawLeft: ");  Serial.print(rawLeft);   Serial.print("  ");
  Serial.print("rawRight: "); Serial.print(rawRight);  Serial.print("  ");
  Serial.print("LeftMotorOutput: ");  Serial.print(LeftMotorOutput);   Serial.print("  ");
  Serial.print("RightMotorOutput: "); Serial.print(RightMotorOutput);  Serial.print("  ");
  Serial.println();
#endif
  
  if (LeftMotorOutput >= 10) {
    digitalWrite(PWM_LEFT_PIN_1, 0);
    analogWrite(PWM_LEFT_PIN_2, LeftMotorOutput);
  } else if (LeftMotorOutput <= -10) {
    digitalWrite(PWM_LEFT_PIN_2, 0);
    analogWrite(PWM_LEFT_PIN_1, abs(LeftMotorOutput));
  } else {
    digitalWrite(PWM_LEFT_PIN_1, 0);
    digitalWrite(PWM_LEFT_PIN_2, 0);
  }
  if (RightMotorOutput >= 10) {
    digitalWrite(PWM_RIGHT_PIN_1, 0);
    analogWrite(PWM_RIGHT_PIN_2, RightMotorOutput);
  } else if (RightMotorOutput <= -10) {
    digitalWrite(PWM_RIGHT_PIN_2, 0);
    analogWrite(PWM_RIGHT_PIN_1, abs(RightMotorOutput));
  } else {
    digitalWrite(PWM_RIGHT_PIN_1, 0);
    digitalWrite(PWM_RIGHT_PIN_2, 0);
  }
}

#endif

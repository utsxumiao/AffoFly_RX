#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "Sonar.h"
#include "Output.h"

#ifdef DC_MOTOR_DIFFERENTIAL_PWM

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
#ifdef DC_MOTOR_ARM_AUX
  uint16_t armAux = 0;
  switch (DC_MOTOR_ARM_AUX) {
    case 1:
      armAux = controlData.Aux1;
      break;
    case 2:
      armAux = controlData.Aux2;
      break;
    case 3:
      armAux = controlData.Aux3;
      break;
    case 4:
      armAux = controlData.Aux4;
      break;
    case 5:
      armAux = controlData.Aux5;
      break;
    case 6:
      armAux = controlData.Aux6;
      break;
    default:
      armAux = 1000;
      break;
  }
  if (armAux < 1200) return;
#endif

  float rawLeft;
  float rawRight;

  float x = map(controlData.Roll, RC_MIN, RC_MAX, DDRIVE_MIN, DDRIVE_MAX);
  float y = map(controlData.Pitch, RC_MIN, RC_MAX, DDRIVE_MIN, DDRIVE_MAX);

#ifdef SONAR
  if (SONAR_DISTANCE_CM <= SONAR_ALARM_DISTANCE && y > 0) {
    y = 0;
  }
#endif

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
      speedRatio = (float)controlData.Aux1 / (float)RC_MAX;
      break;
    case 2:
      speedRatio = (float)controlData.Aux2 / (float)RC_MAX;
      break;
    case 3:
      speedRatio = (float)controlData.Aux3 / (float)RC_MAX;
      break;
    case 4:
      speedRatio = (float)controlData.Aux4 / (float)RC_MAX;
      break;
    case 5:
      speedRatio = (float)controlData.Aux5 / (float)RC_MAX;
      break;
    case 6:
      speedRatio = (float)controlData.Aux6 / (float)RC_MAX;
      break;
    default:
      speedRatio = 1;
      break;
  }
#ifdef DEBUG
  Serial.print("speedRatio: "); Serial.print(speedRatio); Serial.print("  ");
#endif
  LeftMotorOutput *= speedRatio * speedRatio;
  RightMotorOutput *= speedRatio * speedRatio;
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

#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "Output.h"

#ifdef PWM

#include <Servo.h>

Servo servoThrottle, servoYaw, servoPitch, servoRoll, servoAux1, servoAux2;

void Output_init() {
  servoThrottle.attach(PWM_THROTTLE_PIN);
  servoYaw.attach(PWM_YAW_PIN);
  servoPitch.attach(PWM_PITCH_PIN);
  servoRoll.attach(PWM_ROLL_PIN);
  servoAux1.attach(PWM_AUX1_PIN);
  servoAux2.attach(PWM_AUX2_PIN);
}

void Output_data(ControlData controlData) {
  servoThrottle.writeMicroseconds(controlData.Throttle);
  servoYaw.writeMicroseconds(controlData.Yaw);
  servoPitch.writeMicroseconds(controlData.Pitch);
  servoRoll.writeMicroseconds(controlData.Roll);
  servoAux1.writeMicroseconds(controlData.Aux1);
  servoAux2.writeMicroseconds(controlData.Aux2);
}

#endif

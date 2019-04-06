#ifndef CONFIG_H_
#define CONFIG_H_

//#define PWM
#define DC_MOTOR_DIFFERENTIAL_PWM
#ifdef DC_MOTOR_DIFFERENTIAL_PWM
#define DC_MOTOR_ARM_AUX      1
#define DC_MOTOR_SPEED_AUX    2
#endif
//#define CPPM

//#define BUZZER

//#define V_BAT
#ifdef V_BAT
#define V_BAT_ALARM_VOLTAGE   3.6
#endif

//#define SONAR
#ifdef SONAR
#define SONAR_ALARM_DISTANCE  10
#endif

#endif /* CONFIG_H_ */

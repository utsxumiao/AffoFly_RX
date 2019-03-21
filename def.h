#ifndef DEF_H_
#define DEF_H_

/**************************************************************************************/
/***************              Pro Mini Pin Definitions             ********************/
/**************************************************************************************/
// Spare pins: A4 & A5 can be used for I2C device later. 
// Spare pins: A6, A7 input only, no internal pull up/down
// Pin 11 MOSI, 12 MISO, 13 SCK used for NRF24L01 module SPI

#define RED_LED_PIN       A2
#define BLUE_LED_PIN      A3
#ifdef BUZZER
#define BUZZER_PIN        A0
#endif
#ifdef BAT_V
#define BAT_V_PIN         A1
#endif
#ifdef CPPM
#define CPPM_PIN          2
#endif
#ifdef PWM
#define PWM_THROTTLE_PIN  3
#define PWM_YAW_PIN       5
#define PWM_PITCH_PIN     6
#define PWM_ROLL_PIN      9
#define PWM_AUX1_PIN      10
#endif
#define BIND_PIN          4
#define NRF_CE_PIN        7
#define NRF_CSN_PIN       8





/**************************************************************************************/
/***************             General Configurations                ********************/
/**************************************************************************************/

//#define DEBUG
#define PROJECT_NAME        "AffoFly RX"
#define PROJECT_VERSION     "v0.020"
#define RADIO_PIPE          0xE8E8F0F0E1LL
#define TX_IDENTIFIER       "affoflytx"
#define RX_IDENTIFIER       "affoflyrx"
#define RADIO_CHANNEL_LOWER_BOUNDARY    100
#define RADIO_CHANNEL_UPPER_BOUNDARY    125

#define SIGNAL_HIGH_PACKAGE_COUNT       100
#define SIGNAL_MEDIUM_PACKAGE_COUNT     75

#define EEPROM_SETTING_LENGTH           30
#define EEPROM_VERSION_ID               "AffoFly_u5f2mt7t"
#define EEPROM_VERSION_ID_LENGTH        16
#define EEPROM_VERSION_ID_ADDRESS       0
#define EEPROM_TOKEN_ADDRESS            16
#define EEPROM_CHANNEL_ADDRESS          20

#ifdef CPPM
#define CLOCK_MULTIPLIER  1       // 1 for 8MHZ, 2 for 16MHZ
#define CPPM_PIN          2
#define PPM_FRAME_LENGTH  20000
#define PPM_PULSE_LENGTH  400
#define CHANNEL_COUNT     12
#endif

#endif /* DEF_H_ */

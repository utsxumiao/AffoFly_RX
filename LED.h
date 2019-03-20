#ifndef LED_H_
#define LED_H_

void LED_init();
void LED_start(LEDFlashPattern pattern);
void LED_stop();
void LED_flash(uint32_t currentTime);

void setLEDValues(uint8_t step1BlueLedState, uint8_t step1RedLedState, uint32_t step1Duration, 
                  uint8_t step2BlueLedState, uint8_t step2RedLedState, uint32_t step2Duration, 
                  uint8_t step3BlueLedState, uint8_t step3RedLedState, uint32_t step3Duration, 
                  uint8_t step4BlueLedState, uint8_t step4RedLedState, uint32_t step4Duration, 
                  uint8_t step5BlueLedState, uint8_t step5RedLedState, uint32_t step5Duration, 
                  uint8_t step6BlueLedState, uint8_t step6RedLedState, uint32_t step6Duration, 
                  uint16_t repeats);


#endif /* LED_H_ */

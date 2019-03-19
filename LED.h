#ifndef LED_H_
#define LED_H_

void LED_start(LEDFlashPattern pattern);
void LED_stop();
void LED_flash(uint32_t currentTime);

void setLEDValues(uint32_t firstOnDuration, uint32_t firstOffDuration, uint32_t secondOnDuration, uint32_t secondOffDuration, uint32_t thirdOnDuration, uint32_t thirdOffDuration);


#endif /* LED_H_ */
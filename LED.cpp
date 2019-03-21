#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "LED.h"

LedStateStep ledStateSteps[6];
uint16_t ledFlashPatternRepeats = 0;
uint8_t ledStateStepsIndex = 0;
uint32_t ledFlashingTime = 0;

void LED_init() {
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

void LED_start(LEDFlashPattern pattern) {
  switch (pattern) {
    case signalHigh:
      setLEDValues(1,0,1000000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1);
      break;
    case signalMedium:
      setLEDValues(1,0,100000, 0,0,100000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 5);
      break;
    case signalLow:
      setLEDValues(1,0,500000, 0,0,500000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1);
      break;
    case signalNone:
      setLEDValues(1,0,50000, 0,0,1000000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1);
      break;
    case radioSearching:
      setLEDValues(0,1,50000, 0,0,50000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 5);
      break;
    case radioBound:
      setLEDValues(1,1,200000, 0,0,200000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 3);
      break;
    case radioBindingRequired:
      setLEDValues(0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1);
      break;
    case systemStarted:
      setLEDValues(1,1,1000000, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1);
      break;
  }
}

void LED_stop() {
  setLEDValues(0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1);
}

void LED_flash(uint32_t currentTime) {
  if(ledFlashPatternRepeats) {
    if(ledStateStepsIndex == 0) ledFlashingTime = currentTime;
    if(currentTime >= ledFlashingTime) {
      if(ledStateSteps[ledStateStepsIndex].Duration == 0) {
        digitalWrite(BLUE_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        ledStateStepsIndex = 0;
        ledFlashPatternRepeats--;
      } else {
        digitalWrite(BLUE_LED_PIN, ledStateSteps[ledStateStepsIndex].BlueLedState);
        digitalWrite(RED_LED_PIN, ledStateSteps[ledStateStepsIndex].RedLedState);
        ledFlashingTime += ledStateSteps[ledStateStepsIndex].Duration;
        ledStateStepsIndex++;
        if (ledStateStepsIndex > 5) {
          ledStateStepsIndex = 0;
          ledFlashPatternRepeats--;
        }
      }
    }
  }
}

void setLEDValues(uint8_t step1BlueLedState, uint8_t step1RedLedState, uint32_t step1Duration, 
                  uint8_t step2BlueLedState, uint8_t step2RedLedState, uint32_t step2Duration, 
                  uint8_t step3BlueLedState, uint8_t step3RedLedState, uint32_t step3Duration, 
                  uint8_t step4BlueLedState, uint8_t step4RedLedState, uint32_t step4Duration, 
                  uint8_t step5BlueLedState, uint8_t step5RedLedState, uint32_t step5Duration, 
                  uint8_t step6BlueLedState, uint8_t step6RedLedState, uint32_t step6Duration, 
                  uint16_t repeats) {
  ledStateSteps[0] = {step1BlueLedState, step1RedLedState, step1Duration};
  ledStateSteps[1] = {step2BlueLedState, step2RedLedState, step2Duration};
  ledStateSteps[2] = {step3BlueLedState, step3RedLedState, step3Duration};
  ledStateSteps[3] = {step4BlueLedState, step4RedLedState, step4Duration};
  ledStateSteps[4] = {step5BlueLedState, step5RedLedState, step5Duration};
  ledStateSteps[5] = {step6BlueLedState, step6RedLedState, step6Duration};
  ledFlashPatternRepeats = repeats;
  ledStateStepsIndex = 0;
}

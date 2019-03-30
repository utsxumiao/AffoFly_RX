#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "Buzzer.h"
#include <NewPing.h>
#include "Sonar.h"

#ifdef SONAR

uint8_t SONAR_DISTANCE_CM;
NewPing sonar(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE); 
static const uint32_t sonarReadInterval = 50000; // 20HZ
uint32_t previousSonarReadTime = 0;

void Sonar_init() {}

void Sonar_read(uint32_t currentTime) {
  if (currentTime - previousSonarReadTime >= sonarReadInterval) {
    previousSonarReadTime = currentTime;
    SONAR_DISTANCE_CM = sonar.ping_cm();
  }
}

#endif

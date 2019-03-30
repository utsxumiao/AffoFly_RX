#ifndef SONAR_H_
#define SONAR_H_

#ifdef SONAR

extern uint8_t SONAR_DISTANCE_CM;

void Sonar_init();
void Sonar_read(uint32_t currentTime);

#endif

#endif /* SONAR_H_ */

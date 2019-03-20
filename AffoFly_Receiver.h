#ifndef AFFOFLY_RECEIVER_H_
#define AFFOFLY_RECEIVER_H_

extern uint32_t token;
extern uint8_t channel;

void radioInit();
void receiveData(uint32_t currentTime);
void checkPackageCount(uint32_t currentTime);
void checkSignal(uint32_t currentTime);
void resetData();
void radioBind();

#endif /* AFFOFLY_RECEIVER_H_ */

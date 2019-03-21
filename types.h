#ifndef TYPES_H_
#define TYPES_H_

struct ControlData {
  uint32_t Token;
  uint16_t Throttle;
  uint16_t Yaw;
  uint16_t Pitch;
  uint16_t Roll;
  uint16_t Aux1;
  uint16_t Aux2;
  uint16_t Aux3;
  uint16_t Aux4;
  uint16_t Aux5;
  uint16_t Aux6;
  uint16_t Swd1;
  uint16_t Swd2;
};

struct TxBindData {
  char TxIdentifier[10];
  uint32_t Token;
};

struct LedStateStep {
  bool BlueLedState;
  bool RedLedState;
  uint32_t Duration;
};

typedef enum {
  signalHigh,
  signalMedium,
  signalLow,
  signalNone,
  radioSearching,
  radioBound,
  radioBindingRequired,
  systemStarted
} LEDFlashPattern;

#endif /* TYPES_H_ */

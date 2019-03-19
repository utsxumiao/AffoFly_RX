#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "LED.h"
#include "Output.h"
#include "EEPROM.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include "AffoFly_Receiver.h"

uint32_t previousSignalTime = 0;
static const uint32_t signalTimeout = 500000; // 0.5s

uint32_t token = 0;
uint8_t channel = 0;
ControlData controlData;

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  EEPROM_ensureValid();
#ifdef DEBUG
  EEPROM_dumpAll();
#endif

  pinMode(BIND_PIN, INPUT_PULLUP);
  if (digitalRead(BIND_PIN) == LOW) {
    radioBind();
  } else {
    token = EEPROM_readToken();
    channel = EEPROM_readChannel();
    if (token == 0 || channel == 0) {
      // LED indicates binding is required
      while (1) {

      }
    } else {
      radioInit();
      resetData();
      Output_init();
    }
  }
  delay(100);
}

void loop() {
  uint32_t currentTime = micros();
  receiveData(currentTime);
  Output_data(controlData);
}

void radioInit() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);
  radio.setChannel(channel);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(0xE8E8F0F0E1LL);
  radio.startListening();
#ifdef DEBUG
  Serial.print(F("Channel: ")); Serial.println(channel);
  printf_begin();
  radio.printDetails();
#endif
}

void receiveData(uint32_t currentTime) {
  while(radio.available()) {
    radio.read(&controlData, sizeof(ControlData));
    if(controlData.Token == token) {
      previousSignalTime = currentTime;
    }
  }
}

void checkSignal(uint32_t currentTime) {
  if (currentTime - previousSignalTime > signalTimeout) {
    resetData();
  }
}

void resetData() {
  controlData.Throttle = 1000;
  controlData.Yaw = 1500;
  controlData.Pitch = 1500;
  controlData.Roll = 1500;
}

void radioBind() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(0xE8E8F0F0E1LL);
  
  while (1) {
    uint32_t currentTime = micros();
  }
}

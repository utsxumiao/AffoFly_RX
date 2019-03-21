#include "Arduino.h"
#include "types.h"
#include "config.h"
#include "def.h"
#include "LED.h"
#include "Buzzer.h"
#include "Battery.h"
#include "Output.h"
#include "EEPROM.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include "AffoFly_Receiver.h"

static const uint32_t radioReceiveInterval = 2000; //500hz
uint32_t previousRadioReceiveTime = 0;
static const uint32_t checkSignalInterval = 100000; //10hz
uint32_t previousCheckSignalTime = 0;
uint32_t previousSignalTime = 0;
static const uint32_t signalTimeout = 500000; // 0.5s
uint16_t packageCount = 0;
#ifdef DEBUG
uint16_t loopCount = 0;
#endif
static const uint32_t checkPackageCountInterval = 1000000; //1hz
uint32_t previousCheckPackageCountTime = 0;

uint32_t token = 0;
uint8_t channel = 0;
ControlData controlData;
TxBindData txBindData;

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  EEPROM_ensureValid();
#ifdef DEBUG
  EEPROM_dumpAll();
#endif
  LED_init();
  // Turn both LEDs on until next pattern kicks in
  LEDFlashPattern pattern = systemStarted;
  LED_start(pattern);
  LED_flash(micros());
  pinMode(BIND_PIN, INPUT_PULLUP);
  if (digitalRead(BIND_PIN) == LOW) {
#ifdef DEBUG
    Serial.println("Binding...");
#endif
    radioBind();
  } else {
    token = EEPROM_readToken();
    channel = EEPROM_readChannel();
    if (token == 0 || channel == 0) {
      LEDFlashPattern pattern = radioBindingRequired;
      LED_start(pattern);
      LED_flash(micros());
#ifdef DEBUG
      Serial.println("Binding required");
#endif
      while (1) {
        delay(600000);
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
  checkPackageCount(currentTime);
  checkSignal(currentTime);
  LED_flash(currentTime);
  Output_data(controlData);
#ifdef DEBUG
  loopCount++;
#endif
}

void radioInit() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);
  radio.setChannel(channel);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, RADIO_PIPE);
  radio.startListening();
#ifdef DEBUG
  Serial.print(F("Channel: ")); Serial.println(channel);
  printf_begin();
  radio.printDetails();
#endif
}

void receiveData(uint32_t currentTime) {
  if (currentTime - previousRadioReceiveTime >= radioReceiveInterval) {
    previousRadioReceiveTime = currentTime;
    while (radio.available()) {
      radio.read(&controlData, sizeof(ControlData));
      if (controlData.Token == token) {
        previousSignalTime = currentTime;
        packageCount++;
        //#ifdef DEBUG
        //        Serial.print("AUX1: "); Serial.println(controlData.Aux1);
        //#endif
      }
    }
  }
}

void checkSignal(uint32_t currentTime) {
  if (currentTime - previousCheckSignalTime >= checkSignalInterval) {
    previousCheckSignalTime = currentTime;
    if (currentTime - previousSignalTime > signalTimeout) {
      resetData();
#ifdef DEBUG
      Serial.println("Lost signal, control data has been reset.");
#endif
    }
  }
}

void resetData() {
  controlData.Throttle = 1000;
  controlData.Yaw = 1500;
  controlData.Pitch = 1500;
  controlData.Roll = 1500;
}

void checkPackageCount(uint32_t currentTime) {
  if (currentTime - previousCheckPackageCountTime >= checkPackageCountInterval) {
    previousCheckPackageCountTime = currentTime;
#ifdef DEBUG
    Serial.print("Package count: ");  Serial.print(packageCount); Serial.print("  ");
    Serial.print("Loop count: "); Serial.println(loopCount);
    loopCount = 0;
#endif
    LEDFlashPattern pattern;
    if (packageCount >= SIGNAL_HIGH_PACKAGE_COUNT) {
      pattern = signalHigh;
    } else if (packageCount >= SIGNAL_MEDIUM_PACKAGE_COUNT) {
      pattern = signalMedium;
    } else if (packageCount > 0) {
      pattern = signalLow;
    } else {
      pattern = signalNone;
    }
    LED_start(pattern);
    packageCount = 0;
  }
}

void radioBind() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.enableAckPayload();
  radio.openReadingPipe(1, RADIO_PIPE);
  radio.startListening();
  delay(100);
  radio.stopListening();
  bool bound = false;
  while (1) {
    if (!bound) {
      TxBindData txBindData;
      for (uint8_t i = RADIO_CHANNEL_LOWER_BOUNDARY; i <= RADIO_CHANNEL_UPPER_BOUNDARY; i++) {
        radio.setChannel(i);
        radio.startListening();
        radio.writeAckPayload(1, RX_IDENTIFIER, 10);
        delay(10);
        while (radio.available()) {
          radio.read(&txBindData, sizeof(TxBindData));
          Serial.print("id: "); Serial.print(txBindData.TxIdentifier);  Serial.print("  ");
          if (strcmp(txBindData.TxIdentifier, TX_IDENTIFIER) == 0) {
            Serial.print("token: ");  Serial.print(txBindData.Token); Serial.println();
            radio.writeAckPayload(1, RX_IDENTIFIER, strlen(RX_IDENTIFIER));
            EEPROM_writeToken(txBindData.Token);
            EEPROM_writeChannel(i);
            channel = i;
            bound = true;
            break;
          }
        }
        radio.stopListening();

#ifdef DEBUG
        Serial.print(".");
#endif
      }
      LEDFlashPattern pattern = radioSearching;
      LED_start(pattern);
#ifdef DEBUG
      Serial.println();
#endif
    } else {
      LEDFlashPattern pattern = radioBound;
      LED_start(pattern);
#ifdef DEBUG
      Serial.print("Bound on channel: "); Serial.println(channel);
#endif
    }
  }
}

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
      // LED indicates binding is required
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
  checkSignal(currentTime);
  Output_data(controlData);
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
  while (radio.available()) {
    radio.read(&controlData, sizeof(ControlData));
    if (controlData.Token == token) {
      previousSignalTime = currentTime;
#ifdef DEBUG
      Serial.print("AUX1: "); Serial.println(controlData.Aux1);
#endif
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
#ifdef DEBUG
      Serial.println();
#endif
      pinMode(RED_LED_PIN, OUTPUT);
      digitalWrite(RED_LED_PIN, HIGH);
      delay(100);
      digitalWrite(RED_LED_PIN, LOW);
      delay(100);
    } else {
      pinMode(BLUE_LED_PIN, OUTPUT);
      digitalWrite(BLUE_LED_PIN, HIGH);
      delay(100);
      digitalWrite(BLUE_LED_PIN, LOW);
      delay(100);
#ifdef DEBUG
      Serial.print("Bound on channel: "); Serial.println(channel);
#endif
    }
  }
}

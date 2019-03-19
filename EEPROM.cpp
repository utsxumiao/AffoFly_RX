#include "Arduino.h"
#include <EEPROM.h>
#include "types.h"
#include "config.h"
#include "def.h"
#include "EEPROM.h"

void EEPROM_ensureValid() {
  if (!EEPROM_checkVersionId()) {
    EEPROM_loadDefaults();
  }
}

bool EEPROM_checkVersionId() {
  char versionId[EEPROM_VERSION_ID_LENGTH + 1];
  EEPROM.get(EEPROM_VERSION_ID_ADDRESS, versionId);
  versionId[EEPROM_VERSION_ID_LENGTH] = '\0'; // make a valid char array
#ifdef DEBUG
  Serial.print("EEPROM Version ID: ");
  Serial.println(versionId);
  Serial.print("EEPROM Version ID should be: ");
  Serial.println(EEPROM_VERSION_ID);
#endif
  return !strcmp(versionId, EEPROM_VERSION_ID);
}

void EEPROM_loadDefaults() {
#ifdef DEBUG
  Serial.print(F("Loading defaults for EEPROM..."));
#endif
  for (int i = 0 ; i < EEPROM_SETTING_LENGTH ; i++) {
    EEPROM.update(i, 0);
  }
  EEPROM.put(EEPROM_VERSION_ID_ADDRESS, EEPROM_VERSION_ID);
#ifdef DEBUG
  Serial.println(F("Done"));
#endif
}

uint32_t EEPROM_readToken(){
  uint32_t token;
  EEPROM.get(EEPROM_TOKEN_ADDRESS, token);
  return token;
}

void EEPROM_writeToken(uint32_t token) {
  EEPROM.put(EEPROM_TOKEN_ADDRESS, token);
}

uint8_t EEPROM_readChannel() {
  uint8_t channel;
  EEPROM.get(EEPROM_CHANNEL_ADDRESS, channel);
  return channel;
}

void EEPROM_writeChannel(uint8_t channel) {
  EEPROM.put(EEPROM_CHANNEL_ADDRESS, channel);
}

#ifdef DEBUG
void EEPROM_dumpAll() {
  Serial.println("EEPROM DATA:    ");
  for (int i = 0 ; i < EEPROM_SETTING_LENGTH ; i++) {
    Serial.print("  ");
    Serial.print(i);
    Serial.print("=>");
    Serial.print(EEPROM.read(i));
  }
  Serial.println("");
}
#endif

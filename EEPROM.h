#ifndef EEPROM_H_
#define EEPROM_H_

void EEPROM_ensureValid();
bool EEPROM_checkVersionId();
void EEPROM_loadDefaults();
uint32_t EEPROM_readToken();
void EEPROM_writeToken(uint32_t token);
uint8_t EEPROM_readChannel();
void EEPROM_writeChannel(uint8_t channel);
#ifdef DEBUG
void EEPROM_dumpAll();
#endif

#endif /* EEPROM_H_ */

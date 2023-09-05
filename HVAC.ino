#include "HVAC.h"

const uint8_t RELAY_FAN = 14;  // Relay 1 (Fan): D5 (GPIO14)
const uint8_t RELAY_REVERSING_VALVE = 12;  // Relay 2 (Reversing valve): D6 (GPIO12)
const uint8_t RELAY_COMPRESSOR = 13;  // Relay 3 (Compressor): D7 (GPIO13)

uint8_t current_hvac_mode = HVAC_OFF;

void setupHVAC() {
  pinMode(RELAY_FAN, OUTPUT);
  digitalWrite(RELAY_FAN, HIGH);
  pinMode(RELAY_REVERSING_VALVE, OUTPUT);
  digitalWrite(RELAY_REVERSING_VALVE, HIGH);
  pinMode(RELAY_COMPRESSOR, OUTPUT);
  digitalWrite(RELAY_COMPRESSOR, HIGH);
}

void setHVACMode(uint8_t new_mode) {
  if (new_mode == HVAC_COOL) {
    Serial.println("HVAC mode is cooling.");
    digitalWrite(RELAY_FAN, LOW);
    digitalWrite(RELAY_REVERSING_VALVE, LOW);
    digitalWrite(RELAY_COMPRESSOR, LOW);
    current_hvac_mode = HVAC_COOL;
    setFlash(1);
    
  } else if (new_mode == HVAC_HEAT) {
    Serial.println("HVAC mode is heating.");
    digitalWrite(RELAY_FAN, LOW);
    digitalWrite(RELAY_REVERSING_VALVE, HIGH);
    digitalWrite(RELAY_COMPRESSOR, LOW);
    current_hvac_mode = HVAC_HEAT;
    setFlash(2);
    
  } else if (new_mode == HVAC_OFF) {
    Serial.println("HVAC mode is off.");
    digitalWrite(RELAY_FAN, HIGH);
    digitalWrite(RELAY_REVERSING_VALVE, HIGH);
    digitalWrite(RELAY_COMPRESSOR, HIGH);
    current_hvac_mode = HVAC_OFF;
    setFlash(0);
    
  } else if (new_mode == HVAC_FAN) {
    Serial.println("HVAC mode is fan.");
    digitalWrite(RELAY_FAN, LOW);
    digitalWrite(RELAY_REVERSING_VALVE, HIGH);
    digitalWrite(RELAY_COMPRESSOR, HIGH);
    current_hvac_mode = HVAC_OFF;
    setFlash(3);
  }
}

uint8_t getHVACMode() {
  return current_hvac_mode;
}

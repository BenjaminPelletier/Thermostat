#include "HVAC.h"

unsigned long t_last_state_change = 0;
bool state_change_ready = false;
uint8_t user_intent = HVAC_OFF;

const uint8_t STATE_OFF = 0;
const uint8_t STATE_COOLING = 1;
const uint8_t STATE_HEATING = 2;
const uint8_t STATE_TRANSFERRING_HEAT = 3;
uint8_t control_state = STATE_OFF;

bool offActionAvailable() {
  return user_intent == HVAC_COOL || user_intent == HVAC_HEAT;
}

bool coolActionAvailable() {
  return user_intent == HVAC_OFF;
}

bool heatActionAvailable() {
  return user_intent == HVAC_OFF;
}

String getControlDescription() {
  String message = "Unknown state";
  if (user_intent == HVAC_OFF) {
    message = "Off";
    if (control_state == STATE_TRANSFERRING_HEAT) {
      message += " (transferring heat from coils)";
    } else if (control_state == STATE_COOLING) {
      message += " (cooling, giving time for compressor to adjust)";
    } else if (control_state == STATE_HEATING) {
      message += " (heating, giving time for compressor to adjust)";
    }
  } else if (user_intent == HVAC_COOL) {
    message = "Cooling mode";
    if (control_state == STATE_TRANSFERRING_HEAT) {
      message += " (transferring heat from coils)";
    } else if (control_state == STATE_COOLING) {
      message += " (cooling now)";
    } else if (control_state == STATE_HEATING) {
      message += " (heating???)";
    } else if (control_state == STATE_OFF) {
      message += " (room is cool enough, or waiting for compressor)";
    }
  } else if (user_intent == HVAC_HEAT) {
    message = "Heating mode";
    if (control_state == STATE_TRANSFERRING_HEAT) {
      message += " (transferring heat from coils)";
    } else if (control_state == STATE_COOLING) {
      message += " (cooling???)";
    } else if (control_state == STATE_HEATING) {
      message += " (heating now)";
    } else if (control_state == STATE_OFF) {
      message += " (room is warm enough, or waiting for compressor)";
    }
  }
  return message;
}

void setUserIntent(uint8_t new_user_intent) {
  user_intent = new_user_intent;
}

void cool() {
  setHVACMode(HVAC_COOL);
  control_state = STATE_COOLING;
  t_last_state_change = millis();
  state_change_ready = false;
}

void heat() {
  setHVACMode(HVAC_HEAT);
  control_state = STATE_HEATING;
  t_last_state_change = millis();
  state_change_ready = false;
}

void transferHeat() {
  setHVACMode(HVAC_FAN);
  control_state = STATE_TRANSFERRING_HEAT;
  t_last_state_change = millis();
  state_change_ready = false;
}

void idle() {
  setHVACMode(HVAC_OFF);
  control_state = STATE_OFF;
  t_last_state_change = millis();
  state_change_ready = false;
}

void startCoolingIfNecessary() {
  if (getTemperature() >= temperature_target + temperature_hysteresis) {
    cool();
  }
}

void startHeatingIfNecessary() {
  if (getTemperature() <= temperature_target - temperature_hysteresis) {
    heat();
  }
}

void stopCoolingIfCoolEnough() {
  if (getTemperature() <= temperature_target - temperature_hysteresis) {
    transferHeat();
  }
}

void stopHeatingIfWarmEnough() {
  if (getTemperature() >= temperature_target + temperature_hysteresis) {
    transferHeat();
  }
}

void coolOrIdle() {
  if (getTemperature() >= temperature_target + temperature_hysteresis) {
    cool();
  } else {
    idle();
  }
}

void heatOrIdle() {
  if (getTemperature() <= temperature_target - temperature_hysteresis) {
    heat();
  } else {
    idle();
  }
}

void updateControl() {
  unsigned long t = millis();
  if (!state_change_ready) {
    unsigned long dt = t >= t_last_state_change ? t - t_last_state_change : (0xFFFFFFFFl - t_last_state_change) + t;
    if (dt >= state_change_delay) {
      state_change_ready = true;
    }
  }
  if (!state_change_ready) {
    return;
  }

  if (control_state == STATE_OFF) {
    if (user_intent == HVAC_COOL) {
      startCoolingIfNecessary();
    } else if (user_intent == HVAC_HEAT) {
      startHeatingIfNecessary();
    }
    
  } else if (control_state == STATE_COOLING) {
    if (user_intent == HVAC_COOL) {
      stopCoolingIfCoolEnough();
    } else if (user_intent == HVAC_HEAT || user_intent == HVAC_OFF) {
      transferHeat();
    }
    
  } else if (control_state == STATE_HEATING) {
    if (user_intent == HVAC_HEAT) {
      stopHeatingIfWarmEnough();
    } else if (user_intent == HVAC_COOL || user_intent == HVAC_OFF) {
      transferHeat();
    }
    
  } else if (control_state == STATE_TRANSFERRING_HEAT) {
    if (user_intent == HVAC_COOL) {
      coolOrIdle();
    } else if (user_intent == HVAC_HEAT) {
      heatOrIdle();
    } else if (user_intent == HVAC_OFF) {
      idle();
    }
    
  } else {
    Serial.println("Control state error!");
  }
}

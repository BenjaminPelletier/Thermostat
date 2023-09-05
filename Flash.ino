unsigned long t_prev = 0;
uint8_t flash_index = 1;
uint8_t flash_program = 0;

const unsigned int flash_programs[][9] = {
  {2, 1000, 50, 0, 0, 0, 0, 0, 0},
  {4, 1000, 50, 100, 50, 0, 0, 0, 0},
  {6, 1000, 50, 100, 50, 100, 50, 0, 0},
  {8, 1000, 50, 100, 50, 100, 50, 100, 50},
};

void setFlash(uint8_t new_flash_program) {
  flash_program = new_flash_program;
  flash_index = 1;
  digitalWrite(BUILTIN_LED, HIGH);
}

void updateFlash() {
  unsigned long t = millis();
  unsigned long dt = t - t_prev;
  if (dt >= flash_programs[flash_program][flash_index]) {
    t_prev = t;
    if (flash_index % 2 == 0) {
      digitalWrite(BUILTIN_LED, HIGH);
    } else {
      digitalWrite(BUILTIN_LED, LOW);
    }
    flash_index++;
    if (flash_index > flash_programs[flash_program][0]) {
      flash_index = 1;
    }
  }
}

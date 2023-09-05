#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

const int MEASUREMENT_BUFFER = 100;
float measurements[MEASUREMENT_BUFFER];
unsigned int measurement_index = 0;
float measurement_sum = 0.0f;
unsigned int measurement_count = 0;

void setupSensor() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor!");
    while (true) {
      delay(1000);
    }
  }
}

void updateSensor() {
  measurement_sum -= measurements[measurement_index];
  measurements[measurement_index] = bmp.readTemperature();
  measurement_sum += measurements[measurement_index];
  measurement_index++;
  if (measurement_index >= MEASUREMENT_BUFFER) {
    measurement_index = 0;
  }
  if (measurement_count < MEASUREMENT_BUFFER) {
    measurement_count++;
  }
}

float getTemperature() {
  return (measurement_sum / measurement_count) * 9.0f / 5.0f + 32;
}

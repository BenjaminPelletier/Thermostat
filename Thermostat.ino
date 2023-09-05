#include <ESP8266mDNS.h>
#include "Endpoints.h"

float temperature_target = 72.0f;
float temperature_hysteresis = 1.0f;
unsigned long state_change_delay = 2 * 60 * 1000;

void reboot() {
  Serial.println("Restarting...");
  ESP.restart();
}

void setup() {
  delay(1000);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  Serial.begin(57600);
  Serial.println();
  Serial.println("Initializing...");

  Serial.println("Connecting to access point...");
  if (!connectToAP()) {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(1000);
    digitalWrite(BUILTIN_LED, LOW);
    reboot();
  }

  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Setting up mDNS responder...");
  if (!MDNS.begin("playroom")) {
    Serial.println("  Error setting up MDNS responder!");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("Setting up endpoints...");
  setupEndpoints();

  Serial.println("Setting up temperature sensor...");
  setupSensor();

  Serial.println("Setting up HVAC controls...");
  setupHVAC();

  Serial.println("Adding mDNS service...");
  MDNS.addService("http", "tcp", 80);

  Serial.println("Initialization complete.");
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  MDNS.update();
  server.handleClient();
  updateFlash();
  updateSensor();
  updateControl();
}

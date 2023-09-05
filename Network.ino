#include "Network.h"
#include "Private.h"

String wlStatusToString(uint8_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
    default: return "WL_<UNKNOWN>";
  }
}

bool connectToAP() {
  WiFi.begin(ssid, password);
  unsigned long t1 = millis() + CONNECTION_ATTEMPT_DURATION_MS;
  uint8_t wifi_status = WiFi.status();
  uint8_t connected_count = 0;
  while (millis() < t1) {
    delay(500);
    if (wifi_status == WL_CONNECTED) {
      connected_count++;
      if (connected_count >= 5) {
        return true;
      }
    } else {
      connected_count = 0;
    }
    Serial.print("  ");
    Serial.print(wlStatusToString(wifi_status));
    Serial.print(" ");
    Serial.println(connected_count);
    
    wifi_status = WiFi.status();
  }
  return false;
}

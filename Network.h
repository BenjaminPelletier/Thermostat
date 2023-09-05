#ifndef Network_h
#define Network_h

#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiAP.h>
#endif

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
#endif

const unsigned long CONNECTION_ATTEMPT_DURATION_MS = 15000;

#endif

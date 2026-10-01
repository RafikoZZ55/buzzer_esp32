#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "hardware.h"
#include "network.h"

void setup() {
  initNetwork();
  initHardware();
}

void loop() {
  handleClient();
  handleHardware();
}



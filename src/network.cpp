#include <Arduino.h>
#include "network.h"
#include "config.h"
#include "hardware.h"

static WebServer Server(80);

static void setWiFi();
static void handleRoot();
static void handleToggle();
static void setServer();
static void setSerial();

void initNetwork() {
    setSerial();
    setWiFi();
    setServer();
}

static void setWiFi() {
    const unsigned long WiFiConnectionTimeout = 10000;
    unsigned long startAttemptTime = millis();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);
    Serial.println("CONECTING TO WIFI:");

    while (WiFi.status() != WL_CONNECTED && (startAttemptTime + WiFiConnectionTimeout) > millis()){
        Serial.println("...");
        delay(250);
    }

    if(WiFi.status() == WL_CONNECTED){
        Serial.printf("WiFi connected: %s",WiFi.localIP().toString().c_str());
    }
}

static void handleRoot() {
    String html = "<h1>ESP32 alarm</h1>";
    html += "<p> Status: " + String(isOn ? "WŁĄCZONY" : "WYŁĄCZONY") + "</p>";
    html += "<a href=\"/toggle\"/><button style=\"padding:20px; font-size:20px;\">Przelacz alarm</button></a>";
    Server.send(200,"text/html",html);
}

static void handleToggle() {
    toggleAlert();
    Server.sendHeader("Location","/");
    Server.send(303);
}

static void setServer() {
    Server.on("/",handleRoot);
    Server.on("/toggle", handleToggle);
    Server.begin();
    Serial.println("\nSERWER URUCHOMIONY");
}

static void setSerial() {
    Serial.begin(115200);
}

void handleClient() {
    Server.handleClient();
}
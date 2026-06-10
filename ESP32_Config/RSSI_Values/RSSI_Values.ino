#include <WiFi.h>

//    Router Wifi
const char* ssid = "LiDAR";
const char* password = "YoloOS.exe";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  delay(100);
}
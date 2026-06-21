#include <WiFi.h>

HardwareSerial STM32Serial(2);  // Use UART2

//    UART PINS
#define RXD2 16
#define TXD2 17
//    Router Wifi
const char* ssid = "BlueStreetCafe_FreeWifi";
const char* password = "YoloOS.exe.bak";

//    Serial: USB Communication
//    STM32Serial: UART Communication

void setup() {
  //    Wifi Init
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //    Configuring UART
  STM32Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  //    "Configuration Succesfully" message
  Serial.println("ESP32 UART ready");
}

void loop() {
  //    Sending Data
  if (WiFi.status() == WL_CONNECTED) {
    STM32Serial.println(WiFi.RSSI());
    Serial.println(WiFi.RSSI());
  }else{
    STM32Serial.println("---");
    Serial.println("---");
  }
  delay(100);
}
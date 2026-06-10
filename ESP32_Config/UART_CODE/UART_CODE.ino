HardwareSerial STM32Serial(2);  // Use UART2

//    UART PINS
#define RXD2 16
#define TXD2 17

//    Serial: USB Communication
//    STM32Serial: UART Communication

void setup() {
  //    In case of a laptop
  Serial.begin(115200);

  //    Configuring UART
  STM32Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  //    "Configuration Succesfully" message
  Serial.println("ESP32 UART ready");
}

void loop() {
  //    Sending Data
  STM32Serial.println("This is an ESP32 message");
  
  //    Recieving data
  while (STM32Serial.available()) {
    char c = STM32Serial.read();
    Serial.write(c);
  }
  delay(100);
}
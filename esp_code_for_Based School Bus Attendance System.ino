#include <HardwareSerial.h>

HardwareSerial Serial2(2); // Use hardware serial port 2 for communication with Arduino

void setup() {
    Serial.begin(9600); // For debugging purposes
    Serial2.begin(9600, SERIAL_8N1, 15, 16); // RX pin 9, TX pin 10 on ESP32
}

void loop() {
    // Example of sending data to Arduino
    String dataToSend = "Hello from ESP32";
    Serial2.println(dataToSend); // Send data to Arduino
    
    delay(1000); // Adjust delay as needed
}

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM800L(2, 3); // GSM module connected here
String textForSMS;

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  

// Parents' numbers 
const char* f1001 = "+917411034915"; // student1 father cell phone number
const char* f1002 = "+919535438373"; 
const char* f1003 = "+919019541074"; 

void setup() {
  Serial.begin(9600); // For communication with ESP32
  SIM800L.begin(9600); // initialize SIM800L
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522 card
  Serial.println();
  Serial.println(F("Initializing....."));
  delay(1000);
  Serial.println("---------------ATTENDANCE SYSTEM---------------");
  delay(500);
  Serial.println("Scan your Card!!!!");
  delay(1000);
}

void loop() {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println(F("Failed to read card."));
    return;
  }

  // Dump UID
  Serial.print(F("Card UID: "));
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println(cardUID);

  // Dump PICC type
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
      && piccType != MFRC522::PICC_TYPE_MIFARE_1K
      && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("This sample only works with MIFARE Classic cards."));
    return;
  }

  // Defining Cards here
  // Student 1
  if (mfrc522.uid.uidByte[0] == 0xF2 && mfrc522.uid.uidByte[1] == 0x3B && mfrc522.uid.uidByte[2] == 0xE7 && mfrc522.uid.uidByte[3] == 0x54) {
    Serial.println(F("Student 1"));
    const char myMsg[] = "Student 1";
    Serial.println(myMsg); 
    SIM800L.println(myMsg); 
    sendsms(F("Dear Parent,Your ward Varsha is Present"), f1001);
    delay(1000);   

    // Send data to ESP32
    Serial.println(F("Student 1 present:Varsha"));
  }
  // Student 2
  else if (mfrc522.uid.uidByte[0] == 0xEE && mfrc522.uid.uidByte[1] == 0x91 && mfrc522.uid.uidByte[2] == 0x3F && mfrc522.uid.uidByte[3] == 0x1D) {
    Serial.println(F("Student 2"));
    const char myMsg[] = "Student 2";
    Serial.println(myMsg); 
    SIM800L.println(myMsg); 
    sendsms(F("Dear Parent,Your ward Soundarya is Present"), f1002);
    delay(1000);   

    // Send data to ESP32
    Serial.println(F("Student 2 present:Soundarya"));
  }
  else {
    Serial.println(F("Unregistered user"));
  }
}

void sendsms(String message, const char* number) {
  String mnumber = "AT + CMGS = \"" + String(number) + "\""; 
  SIM800L.print(F("AT+CMGF=1\r"));                   
  delay(1000);
  SIM800L.println(mnumber);  // recipient's mobile number, in international format
  delay(1000);
  SIM800L.println(message);  // message to send
  delay(1000);
  SIM800L.println((char)26); // End AT command with a ^Z, ASCII code 26
  delay(1000); 
  SIM800L.println();
  delay(100);               // Give module time to send SMS
}

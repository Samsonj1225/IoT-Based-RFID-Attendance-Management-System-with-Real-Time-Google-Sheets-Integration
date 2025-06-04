#include <SPI.h>
#include <MFRC522.h>

const uint8_t RST_PIN = D3;   // Reset pin for RFID module
const uint8_t SS_PIN = D4;    // Slave select pin for RFID module

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;           // Create MIFARE key instance

int blockNum = 4;           // Initial memory block to write
byte bufferLen = 18;
byte readBlockData[18];     // Buffer to hold read data
MFRC522::StatusCode status; // Status of RFID operations

void setup() {
  Serial.begin(9600);
  SPI.begin();                // Initialize SPI bus
  mfrc522.PCD_Init();         // Initialize RFID reader
  Serial.println("Scan a MIFARE 1K Tag to write data...");
}

void loop() {
  // Initialize key to default (0xFF)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println("\n**Card Detected**");

  // Print UID of card
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.print("\nPICC type: ");
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];
  byte len;

  Serial.setTimeout(20000L); // Wait for user input

  // Roll Number
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Enter Student Roll No, ending with #"));
  len = Serial.readBytesUntil('#', (char *)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  WriteDataToBlock(4, buffer);  // Write to block 4
  ReadDataFromBlock(4, readBlockData);
  dumpSerial(4, readBlockData);

  // Name
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Enter Student Name, ending with #"));
  len = Serial.readBytesUntil('#', (char *)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  WriteDataToBlock(5, buffer);  // Write to block 5
  ReadDataFromBlock(5, readBlockData);
  dumpSerial(5, readBlockData);

  // Department
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Enter Department, ending with #"));
  len = Serial.readBytesUntil('#', (char *)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  WriteDataToBlock(6, buffer);  // Write to block 6
  ReadDataFromBlock(6, readBlockData);
  dumpSerial(6, readBlockData);

  // Year
  Serial.println(F("---------------------------------------"));
  Serial.println(F("Enter Year, ending with #"));
  len = Serial.readBytesUntil('#', (char *)buffer, 16);
  for (byte i = len; i < 16; i++) buffer[i] = ' ';
  WriteDataToBlock(8, buffer);  // Write to block 8
  ReadDataFromBlock(8, readBlockData);
  dumpSerial(8, readBlockData);
}

// Function to write data to a specific block
void WriteDataToBlock(int blockNum, byte blockData[]) {
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                     blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Writing to Block failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
}

// Function to read data from a block
void ReadDataFromBlock(int blockNum, byte readBlockData[]) {
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                     blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Authentication failed for Read: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
}

// Function to print data read from a block
void dumpSerial(int blockNum, byte blockData[]) {
  Serial.print("\nData saved on block ");
  Serial.print(blockNum);
  Serial.print(": ");
  for (int j = 0; j < 16; j++) {
    Serial.write(readBlockData[j]);
  }
  Serial.println();
  // Clear buffer after read
  for (int i = 0; i < sizeof(readBlockData); ++i)
    readBlockData[i] = (char)0;
}

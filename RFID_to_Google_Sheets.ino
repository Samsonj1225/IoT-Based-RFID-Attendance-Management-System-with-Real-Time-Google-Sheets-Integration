#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPSRedirect.h>

// Google Script ID from Google Apps Script URL
const char *GScriptId = "************************";

const char* ssid = "*******";       // WiFi SSID
const char* password = "*****"; // WiFi password

String payload_base = "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

const char* host = "script.google.com";
const int httpsPort = 443;

String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

String student_Rollno;

int blocks[] = {4, 5, 6, 8, 9}; // RFID blocks to read from
#define total_blocks (sizeof(blocks) / sizeof(blocks[0]))

#define RST_PIN 4
#define SS_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

int blockNum = 2;
byte bufferLen = 18;
byte readBlockData[18];

void setup() {
  Serial.begin(9600);
  delay(10);
  SPI.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to "); Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP address: "); Serial.println(WiFi.localIP());

  // Create HTTPSRedirect client
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();  // Disable certificate verification
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  Serial.println("Connecting to script host...");
  bool flag = false;
  for (int i = 0; i < 5; i++) {
    if (client->connect(host, httpsPort)) {
      flag = true;
      Serial.println("Connected to Google Script.");
      delay(2000);
      break;
    } else {
      Serial.println("Connection failed. Retrying...");
    }
  }

  if (!flag) {
    Serial.println("Could not connect to script server.");
    delay(5000);
    return;
  }

  delete client;
  client = nullptr;
}

void loop() {
  static bool flag = false;
  if (!flag) {
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
    flag = true;
  }

  if (client && !client->connected()) {
    client->connect(host, httpsPort);
  }

  Serial.println("Scan your Tag...");
  mfrc522.PCD_Init(); // Initialize RFID

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.println(F("Reading data from RFID..."));
  String values = "", data;

  // Read each block and prepare CSV-style payload
  for (byte i = 0; i < total_blocks; i++) {
    ReadDataFromBlock(blocks[i], readBlockData);
    data = String((char*)readBlockData);
    data.trim();

    if (i == 0) {
      student_Rollno = data;
      values = "\"" + data + ",";
    } else if (i == total_blocks - 1) {
      values += data + "\"}";
    } else {
      values += data + ",";
    }
  }

  payload = payload_base + values;
  Serial.println("Sending data to Google Sheets...");
  Serial.println(payload);

  // Send POST request to Google Apps Script
  if (client->POST(url, host, payload)) {
    Serial.println("Roll no: " + student_Rollno);
    Serial.println("Marked Present");
  } else {
    Serial.println("Failed to send data");
  }

  delay(3000); // Small delay before next read
}

// Function to read data from RFID block
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
  } else {
    readBlockData[16] = ' ';
    readBlockData[17] = ' ';
    Serial.println("Block was read successfully");
  }
}

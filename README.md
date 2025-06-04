# IoT-Based-RFID-Attendance-Management-System-with-Real-Time-Google-Sheets-Integration

This project is a contactless RFID-based attendance system using an ESP32 and MFRC522 RFID reader that records student data directly into a Google Sheet via App Script. It allows you to write student details (Roll Number, Name, Department, Year) onto RFID cards and later scan them to automatically log attendance with a timestamp. Designed for real-time tracking, the system integrates Wi-Fi and Google Sheets for seamless data storage and easy access.

Steps for Google Sheets App Script Setup:
1.	Search “Google Sheets” in your browser or go to:
 https://docs.google.com/spreadsheets
2.	Click on Blank spreadsheet.
3.	Rename it to something meaningful (e.g., RFID Attendance).
4.	In row 1, enter the following headers:
Roll Number | Name | Department | Year | Timestamp
5.	Click on Extensions > Apps Script.
6.	Delete the default code in the editor and paste the provided App Script code.
7.	Now go to your spreadsheet that you have create in your sheet URL, find the ID between /spreadsheets/d/ and /edit:
https://docs.google.com/spreadsheets/d/**YOUR_SHEET_ID**/edit
Copy and paste this ID into the App Script code (Line 2).
8.	Click the Save icon in the Apps Script editor.
9.	Click on Deploy > New deployment.
10.	Select “Web app” as the deployment type.
11.	Under “Who has access”, choose “Anyone”.
12.	Click Deploy and authorize the permissions.
13.	Copy the Web App URL .

Steps for RFID_Data_Writing (Writing Data to Card):
1.	Open the RFID_Data_Writing Arduino File.
2.	Connect the RFID reader to your ESP8266/ESP32 board and plug the board into your PC via USB.
3.	In Arduino IDE, go to Tools > Board and select your ESP board. Then choose the correct COM port under Tools > Port.
4.	Upload the sketch to your board.
5.	Open Serial Monitor.
6.	Hold the tag near the RFID reader (Do not move or remove during write).
7.	Enter Details in Serial Monitor. After each input, end the line with #. For example:
Eg:
12345#
Samson J#
ECE#
3rd Year#
8.	Once data is written and confirmed in the Serial Monitor, remove the tag.
9.	Repeat for the next RFID card.


Steps for RFID_to_Google_Sheets (Sending Data to Sheet):
1.	Open the RFID_to_Google_Sheets Arduino File.
2.	Replace the placeholders with:
	Your Wi-Fi SSID
	Your Wi-Fi Password
	The GScriptID (Web App URL) from earlier
3.	Upload the code to your ESP8266/ESP32 board.
4.	When a tag is scanned, the data will be fetched from the card and sent to Google Sheets via the Web App.
5.	The real time data entries can be viewed in the corresponding columns. 

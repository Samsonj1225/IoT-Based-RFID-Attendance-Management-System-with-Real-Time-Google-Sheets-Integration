// Enter Spreadsheet ID here
var SS = SpreadsheetApp.openById(*************************************');
var timezone = "Asia/Kolkata";
var hours = 0;
var str = "";

function doPost(e) {
  var parsedData;
  var result = {};
  
  try { 
    parsedData = JSON.parse(e.postData.contents);
  } 
  catch(f){
    return ContentService.createTextOutput("Error in parsing request body: " + f.message);
  }
   
  if (parsedData !== undefined) {
    var flag = parsedData.format;
    if (flag === undefined) {
      flag = 0;
    }
    
    var sheet = SS.getSheetByName(parsedData.sheet_name); // sheet name to publish data to is specified in Arduino code
    var dataArr = parsedData.values.split(","); // creates an array of the values to publish 
         
    var Curr_Date = new Date(new Date().setHours(new Date().getHours() + hours));
    var Curr_Time = Utilities.formatDate(Curr_Date, timezone, 'HH:mm:ss');

    var value0 = dataArr[0]; // Student ID
    var value1 = dataArr[1]; // First Name
    var value2 = dataArr[2]; // Last Name
    var value3 = dataArr[3]; // Phone Number
  
    // read and execute command from the "payload_base" string specified in Arduino code
    switch (parsedData.command) {
      
      case "insert_row":
         
         sheet.insertRows(2); // insert full row directly below header text
         
         sheet.getRange('A2').setValue(Curr_Date); // publish current date to cell A2
         sheet.getRange('B2').setValue(Curr_Time); // publish current time to cell B2
         sheet.getRange('C2').setValue(value0);   // publish Student ID to cell C2
         sheet.getRange('D2').setValue(value1);   // publish First Name to cell D2
         sheet.getRange('E2').setValue(value2);   // publish Last Name to cell E2
         sheet.getRange('F2').setValue(value3);   // publish Phone Number to cell F2
         
         str = "Success"; // string to return back to Arduino serial console
         SpreadsheetApp.flush();
         break;
         
      case "append_row":
         
         var publish_array = new Array(); // create a new array
         
         publish_array[0] = Curr_Date; // add current date to position 0 in publish_array
         publish_array[1] = Curr_Time; // add current time to position 1 in publish_array
         publish_array[2] = value0;     // add Student ID to position 2 in publish_array
         publish_array[3] = value1;     // add First Name to position 3 in publish_array
         publish_array[4] = value2;     // add Last Name to position 4 in publish_array
         
         sheet.appendRow(publish_array); // publish data in publish_array after the last row of data in the sheet
         
         str = "Success"; // string to return back to Arduino serial console
         SpreadsheetApp.flush();
         break;     
 
    }
    
    return ContentService.createTextOutput(str);
  } // endif (parsedData !== undefined)
  
  else {
    return ContentService.createTextOutput("Error! Request body empty or in incorrect format.");
  }
}

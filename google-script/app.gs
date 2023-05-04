var ss = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/1RcYlmxQAznj83vKuDPzCUz2ENzumkxtC3WUML4Wr3Mg/edit#gid=0");

function doPost(e) {

  var client = e.parameter.client;

  //If post is sent from app
  if (client == "app") {
    return addItemApp(e);
  }

  //If post is sent from arduino
  else if (client == "arduino") {
    return addItemArduino(e);
  }
  return ContentService.createTextOutput('Data posting failed');
}

function addItemApp(e) {
  var sheet = ss.getSheetByName('appToDevice');
  var waterAmount = e.parameter.vWaterAmount;
  sheet.deleteRow(2);
  var water = "yes";
  switch (e.parameter.plant) {
    case "one":
        sheet.getRange("A2").setValue(water);
        sheet.getRange("B2").setValue(waterAmount);
        break;
    case "two":
        sheet.getRange("C2").setValue(water);
        sheet.getRange("D2").setValue(waterAmount);
        break;
    case "three":
        sheet.getRange("E2").setValue(water);
        sheet.getRange("F2").setValue(waterAmount);
        break;
    case "four":
        sheet.getRange("G2").setValue(water);
        sheet.getRange("H2").setValue(waterAmount);
        break;
  }

  return ContentService.createTextOutput("Success").setMimeType(ContentService.MimeType.TEXT);
}

function addItemArduino(e) {
  var data = e.parameter; // retrieve the data sent by the Arduino
  if (data.toWater == "true") {
    var sheet = ss.getSheetByName('appToDevice');
    var row = sheet.getRange('2:2');
    row.clear();
    return ContentService.createTextOutput('Success').setMimeType(ContentService.MimeType.TEXT);
  }
  else {
    var sheet = ss.getSheetByName('deviceToApp');
    sheet.appendRow([data.moistureLevel1, data.moistureLevel2, data.moistureLevel3, 
      data.moistureLevel4, data.waterLevel, new Date()]);
    return ContentService.createTextOutput('Success').setMimeType(ContentService.MimeType.TEXT);
  }
}

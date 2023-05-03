#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <string>

String NextionCom;
int isWifiPage;
bool wifi = false;
String setLarge = "";
String setMedium = "";
String setSmall= "";
String Water = "";

char serverPost[] = "script.google.com";
char serverGet[] = "sheets.googleapis.com";
int port = 443;

// Replace the script URL with your own Apps Script URL
String scriptURL = "/macros/s/AKfycbzeDGdQXSf6AmGz1ukGdI66aqPnLp2WNJXN6wtr68frVAtlIbtQVm2LH4UMH-wcGdaDaQ/exec";
String apiURL = "/v4/spreadsheets/1RcYlmxQAznj83vKuDPzCUz2ENzumkxtC3WUML4Wr3Mg/values/appToDevice?key=API KEY GOES HERE";
String contentType = "application/x-www-form-urlencoded";
String clientType = "arduino";
String waterLevel = "NA";
String moistureLevel = "NA";

WiFiSSLClient client;
HttpClient httpPost(client, serverPost, port);
HttpClient httpGet(client, serverGet, port);

// Some default delay values for small, medium, and large.
// Flow rate is 11.34mL/s
int small = 1000;
int medium = 2000;
int large = 3000;

//Relay Code
int relayPin1 = 2; //This is the Arduino Pin that will control Relay #1
int relayPin2 = 3; //This is the Arduino Pin that will control Relay #2
int relayPin4 = 4; //This is the Arduino Pin that will control Relay #3
int relayPin5 = 5; //This is the Arduino Pin that will control Relay #4
int relayPin6 = 6; //This is the Arduino Pin that will control Relay #5
int i = 0;
int f = 0;

//Initial Definitions
const int dry0 = 902;    //placeholder, need to test
const int wet0 = 439;    //placeholder, need to test

const int dry1 = 895;
const int wet1 = 438;

const int dry2 = 902;
const int wet2 = 437;

const int dry3 = 896;
const int wet3 = 448;

const int dry4 = 902;
const int wet4 = 437;

const int dry5 = 896;
const int wet5 = 448;

const int dry6 = 902;
const int wet6 = 437;

const int dry7 = 896;
const int wet7 = 448;



unsigned long previousMillis = 0; // Variable to store the previous millis value
const unsigned long interval = 600000; // Interval in milliseconds (20 seconds)

#define trigPin 8
#define echoPin 7

#define BUFFSIZE 10
int buffer[BUFFSIZE];
int bufferIndex = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(trigPin, OUTPUT); //USRF
  pinMode(echoPin, INPUT);  //USRF

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);  
  pinMode(7, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin5, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  pinMode(relayPin6, OUTPUT);

  initBuffer();
  
}

// Calculate height from ultrasonic range finder. Only works with the
// original tank as percentage is calibrated on that specified height. 
int findHeight() {
  float duration, inches, cm;
  int percent;

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);

  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  inches = 9.5-inches;
  percent = inches * 12.739;
  if (percent <= 0) {
    percent = 0;
  }
  if (percent >= 100) {
    percent = 100;
  }
  putVal(percent);
  return getAvg();
}

// Get average value from circular buffer for tank value. 
int getAvg() {
    int j;
    int total = 0;
    for (j = 0; j < BUFFSIZE; j++) {
        total = total + buffer[j];
    }
    return total/BUFFSIZE;
}

// Initialize circular buffer for tank level.
void initBuffer() {
    int i;
    for (i = 0; i < BUFFSIZE; i++) {
        buffer[i] = 0;
    }
}

// Put value into circular buffer for tank level.
void putVal(int percent) {
    buffer[bufferIndex++] = percent;
    if (bufferIndex >= BUFFSIZE) {
        bufferIndex = 0;
    }
}

// Set moisture values array
int* findMoisture(){
  static int moisture[4] = {0,0,0,0}; 
  moisture[0] = map(analogRead(A0), wet0, dry0, 100, 0);
  moisture[1] = map(analogRead(A1), wet1, dry1, 100, 0);
  moisture[2] = map(analogRead(A2), wet2, dry2, 100, 0);
  moisture[3] = map(analogRead(A3), wet3, dry3, 100, 0);

  if (moisture[0]<=0){
    moisture[0] = 0;  
  }
  if (moisture[0]>=100){
    moisture[0] = 100;  
  }
  if (moisture[1]<=0){
    moisture[1] = 0;  
  }
  if (moisture[1]>=100){
    moisture[1] = 100;  
  }
  if (moisture[2]<=0){
    moisture[2] = 0;  
  }
  if (moisture[2]>=100){
    moisture[2] = 100;  
  }
  if (moisture[3]<=0){
    moisture[3] = 0;  
  }
  if (moisture[3]>=100){
    moisture[3] = 100;  
  }
  return moisture;
}

void loop() {
  unsigned long currentMillis = millis();

  // Turns off water pumps
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  digitalWrite(relayPin4, HIGH);
  digitalWrite(relayPin5, HIGH);
  digitalWrite(relayPin6, HIGH);

  command();
  updateAll();

  // Check if the specified interval has passed
  // Makes sure that the sheet is only updated every 30 minutes
  // to avoid conflict with HTTP requests.
  if (currentMillis - previousMillis >= interval) {

    // Update the previousMillis value to the current time
    previousMillis = currentMillis;
    int* moisture = findMoisture();

    delay(100);
    /* Uncomment these lines for debugging puporses
    -----------------------------------------------
    Serial.print("Tank Level: ");
    Serial.println(findHeight());
    Serial.print("Plant 1: ");
    Serial.print(moisture[0]);
    Serial.println("% "); 
    Serial.print("Plant 2: ");
    Serial.print(moisture[1]);
    Serial.println("% ");
    Serial.print("Plant 3: ");
    Serial.print(moisture[2]);
    Serial.println("% ");
    Serial.print("Plant 4: ");
    Serial.print(moisture[3]);
    Serial.println("% ");
    Serial.print("Water Level: ");
    Serial.println(findHeight());  
    previousMillis = currentMillis; 
    */
    if (wifi) {
      Serial.println("Posting data...");
      postData("client=" + clientType + "&waterLevel=" + findHeight() + "&moistureLevel1=" + moisture[0] + 
      "&moistureLevel2=" + moisture[1] + "&moistureLevel3=" + moisture[2] + "&moistureLevel4=" + moisture[3]);
    }
  }

}

// Posting data to the Google Sheet
void postData(String data) {
  httpPost.beginRequest();
  httpPost.post(scriptURL, contentType, data);
  int statusCode = httpPost.responseStatusCode();
  String response = httpPost.responseBody();
  httpPost.endRequest();
}


// Handles all requests from the app to water the plant. 
void checkWater() {
  httpGet.beginRequest();
  httpGet.get(apiURL);
  httpGet.endRequest();

  String response = httpGet.responseBody();
  const size_t CAPACITY = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 8*JSON_ARRAY_SIZE(8) + 280;
  StaticJsonDocument<CAPACITY> doc;

  DeserializationError error = deserializeJson(doc, response);

  JsonArray values = doc["values"].as<JsonArray>();
  const char **water = (const char**) malloc(4 * sizeof(char*));
  water[0] = values[1][0].as<const char*>();
  water[1] = values[1][2].as<const char*>();
  water[2] = values[1][4].as<const char*>();
  water[3] = values[1][6].as<const char*>();

  if (strcmp(water[0], "yes") == 0) {
    const char* quantity = values[1][1].as<const char*>();
    if (strcmp(quantity, "small") == 0) {
      Serial.println("Pouring Plant 1 small");
      pourPlant("one", small);
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "medium") == 0) {
      Serial.println("Pouring Plant 1 medium");
      pourPlant("one", medium);
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "large") == 0) {
      Serial.println("Pouring Plant 1 large");
      pourPlant("one", large);   
      postData("client=" + clientType + "&toWater=true");
    }
  }
  if (strcmp(water[1], "yes") == 0) {
    const char* quantity = values[1][3].as<const char*>();
    if (strcmp(quantity, "small") == 0) {
      Serial.println("Pouring Plant 2 small");
      pourPlant("two", small);
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "medium") == 0) {
      Serial.println("Pouring Plant 2 medium");
      pourPlant("two", medium);
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "large") == 0) {
      Serial.println("Pouring Plant 2 large");
      pourPlant("two", large);       
      postData("client=" + clientType + "&toWater=true");
    }
  }
  if (strcmp(water[2], "yes") == 0) {
    const char* quantity = values[1][5].as<const char*>();
    if (strcmp(quantity, "small") == 0) {
      Serial.println("Pouring Plant 3 small");
      pourPlant("three", small);   
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "medium") == 0) {
      Serial.println("Pouring Plant 3 medium");
      pourPlant("three", medium);   
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "large") == 0) {
      Serial.println("Pouring Plant 3 large");
      pourPlant("three", large);   
      postData("client=" + clientType + "&toWater=true");
    }
  }
  if (strcmp(water[3], "yes") == 0) {
    const char* quantity = values[1][7].as<const char*>();
    if (strcmp(quantity, "small") == 0) {
      Serial.println("Pouring Plant 4 small");
      pourPlant("four", small);   
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "medium") == 0) {
      Serial.println("Pouring Plant 4 medium");
      pourPlant("four", medium);   
      postData("client=" + clientType + "&toWater=true");
    }
    if (strcmp(quantity, "large") == 0) {
      Serial.println("Pouring Plant 4 large");
      pourPlant("four", large);      
      postData("client=" + clientType + "&toWater=true");
    }
  }
  free(water);
}

void printEncryptionType(int thisType){
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.print("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.print("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.print("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.print("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.print("Auto");
      break;
    case ENC_TYPE_UNKNOWN:
    default:
      Serial.print("Unknown");
      break;
  }
}

void print2Digits(byte thisByte) {
  if (thisByte < 0xF) {
    Serial.print("0");
  }
  Serial.print(thisByte, HEX);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

// Parses the command recieved from the touch screen to determine
// what action to take. 
void command(){
  String plant = "";
  String waterSize = "";
  String waterSizeConv = "";
  int size;
  NextionCom = getCommand();
  char* ptr = strchr(NextionCom.c_str(), 26);

  // Check if the app has made a water request IF connected to wifi and
  // there was no command recieved from the touch screen. 
  if (ptr == NULL && wifi == true) {
    checkWater();
  } 

  NextionCom = NextionCom.substring(3, NextionCom.length()-3);

  // See what command was recieved from the touch screen.
  Serial.println(NextionCom);

  // Wifi debugging
  if(NextionCom == "WiFiPage"){
    isWifiPage = 1;
  }
  else if(NextionCom == "NoWifiPage"){ //Wifi Page back button hit
    isWifiPage = 0;
  }

  plant = NextionCom.substring(0, 18);
  waterSize = NextionCom.substring(6,7);

  // Scan for networks
  if(NextionCom == "Scan"){
    listNetworks();
  }
  
  // Next two if statements are how the device connects to wifi.
  if(NextionCom.substring(0,7) == "Connect"){
    typedPW = NextionCom.substring(8, NextionCom.length());
    for(int i = 0; i<typedPW.length(); i++) {
      if (typedPW[i] == 26) {
        typedPW[i] = '\0';
      }
      else {
        break;
      }
    }
    Serial.print("PASSWORD: ");
    Serial.println(typedPW);
    while (WiFi.begin(NTWK.c_str(), typedPW.c_str()) != WL_CONNECTED) {
      Serial.println("Connecting to WiFi...");
      delay(1000);
    }
    Serial.println("WiFi connected");
    wifi = true;
  }
  if(NextionCom.substring(0, 5) =="NetID") {
    NTWK = NextionCom.substring(6, NextionCom.length());
    for(int i = 0; i<NTWK.length(); i++) {
      if (NTWK[i] == 26) {
        NTWK[i] = '\0';
      }
      else {
        break;
      }
    }
    Serial.print("NETID: ");
    Serial.println(NTWK);
  }

  // Setting mL amount for Small, Medium, Large
  if(NextionCom.substring(0,5) == "Large") {
    setLarge =  NextionCom.substring(5, NextionCom.length());
    Serial.print("Setting large...");
    Serial.println(setLarge);
    size = atoi(setLarge.c_str());
    sizeSetLarge(size);
  }
  else if(NextionCom.substring(0,6) == "Medium") {
    setMedium =  NextionCom.substring(6, NextionCom.length());
    Serial.print("Setting medium...");
    Serial.println(setMedium);
    size = atoi(setMedium.c_str());
    sizeSetMedium(size);
  }
  else if(NextionCom.substring(0,5) == "Small") {
    setSmall =  NextionCom.substring(5, NextionCom.length());
    Serial.print("Setting small...");
    Serial.println(setSmall);
    size = atoi(setSmall.c_str());
    sizeSetSmall(size);
  }

  //PLANT 1
  if (NextionCom == "Water1L" || NextionCom == "Water1M" || NextionCom == "Water1S"){
      if (NextionCom == "Water1L") {
        pourPlant("one", large);
      }
      if (NextionCom == "Water1M") {
        pourPlant("one", medium);
      }
      if (NextionCom == "Water1S") {
        pourPlant("one", small);
      }

      Serial.print("scrollT.txt=");
      Serial.print("\"");
      Serial.print("Plant 1 Watered, Size: ");
      Serial.print(waterSizeConv);
      Serial.print("\"");
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
  }

  //PLANT 2
  else if(NextionCom == "Water2L" || NextionCom == "Water2M" || NextionCom == "Water2S"){
      if (NextionCom == "Water2L") {
        pourPlant("two", large);
      }
      if (NextionCom == "Water2M") {
        pourPlant("two", medium);
      }
      if (NextionCom == "Water2S") {
        pourPlant("two", small);
      }
      
      Serial.print("Plant 2 Watered, Size: ");
      Serial.print(waterSizeConv);
      Serial1.print("scrollT.txt=");
      Serial1.print("\"");
      Serial1.print("Plant 2 Watered, Size: ");
      Serial1.print(waterSizeConv);
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
  }

  //PLANT 3
  else if(NextionCom == "Water3L" || NextionCom == "Water3M" || NextionCom == "Water3S"){
      if (NextionCom == "Water3L") {
        pourPlant("three", large);
      }
      if (NextionCom == "Water3M") {
        pourPlant("three", medium);
      }
      if (NextionCom == "Water3S") {
        pourPlant("three", small);
      }
      
      Serial.print("Plant 3 Watered, Size: ");
      Serial.print(waterSizeConv);
      Serial1.print("scrollT.txt=");
      Serial1.print("\"");
      Serial1.print("Plant 3 Watered, Size: ");
      Serial1.print(waterSizeConv);
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
  }
  
  //PLANT 4
  else if(NextionCom == "Water4L" || NextionCom == "Water4M" || NextionCom == "Water4S"){
      if (NextionCom == "Water4L") {
        pourPlant("four", large);
      }
      if (NextionCom == "Water4M") {
        pourPlant("four", medium);
      }
      if (NextionCom == "Water4S") {
        pourPlant("four", small);
      }
      Serial.print("Plant 4 Watered, Size: ");
      Serial.print(waterSizeConv);
      Serial1.print("scrollT.txt=");
      Serial1.print("\"");
      Serial1.print("Plant 4 Watered, Size: ");
      Serial1.print(waterSizeConv);
      Serial1.print("\"");
      Serial1.write(0xff);
      Serial1.write(0xff);
      Serial1.write(0xff);
  }
}

// Reads the command read from the Serial1 from the touch screen.
String getCommand(){
  String inputString = "";

  // Read the input data from the serial port and append it to the string
  while (Serial1.available()) {
    char c = Serial1.read();
    if(c != 26){
      inputString += c;
    }
  }

  // Wait for a short period of time before reading again
  delay(100);
  return inputString;
}


// Updates the tank graphic on the touch screen
void updateAll(){
  int SensorVal = 5;
  delay(500);
  Serial1.print("resovoir.txt=");
  Serial1.print("\"");
  Serial1.print(findHeight());
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.print("tank.val=");
  //Serial1.print("\"");
  Serial1.print(findHeight());
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void isConnected(){
  delay(500);
  Serial1.print("connected.txt=");
  Serial1.print("Connected!");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void notConnected(){
  delay(500);
  Serial1.print("connected.txt=");
  Serial1.print("Not Connected");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void sendWifiNTWRK(){
  Serial1.print("ntwk.txt=");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void listNetworks() {
  // scan for nearby networks:
  char networks[1000] = "";
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    notConnected();
    Serial.print("Cannot Connect");
    while (true);
  }
  
  
  Serial.print("number of available networks: ");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    strcat(networks, WiFi.SSID(thisNet));
    strcat(networks, "\r\n");
  }
  Serial.print(networks);
  Serial1.print("ntwk.txt=");
  Serial1.print("\"");
  Serial1.print(networks);
  Serial1.print("\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void sizeSetSmall(int val) {
  float dec = (float) val / 11.34;
  small = (int) (dec * 1000);
  Serial.print("Small set: ");
  Serial.println(small);
}

void sizeSetMedium(int val) {
  float dec = (float) val / 11.34;
  medium = (int) (dec * 1000);
  Serial.print("Medium set: ");
  Serial.println(medium);
}

void sizeSetLarge(int val) {
  float dec = (float) val / 11.34;
  large = (int) (dec * 1000);
  Serial.print("Large set: ");
  Serial.println(large);
}

// Pour a desired amount of water to specified plant.
// Works for commands from the screen and the app.
void pourPlant(String plant, int amount) {
  if (plant == "one") {    
    Serial.print("Pour amount delay: ");
    Serial.println(amount);
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin5, LOW);
    delay(amount);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin1, HIGH);
  }
  else if (plant == "two") {
    Serial.print("Pour amount delay: ");
    Serial.println(amount);
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin5, LOW);
    delay(amount);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin2, HIGH);
  }
  else if (plant == "three") {
    Serial.print("Pour amount delay: ");
    Serial.println(amount);
    digitalWrite(relayPin4, LOW);
    digitalWrite(relayPin5, LOW);
    delay(amount);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin4, HIGH);
  }
  else if (plant == "four") {
    Serial.print("Pour amount delay: ");
    Serial.println(amount);
    digitalWrite(relayPin6, LOW);
    digitalWrite(relayPin5, LOW);
    delay(amount);
    digitalWrite(relayPin5, HIGH);
    digitalWrite(relayPin6, HIGH);
  }
}
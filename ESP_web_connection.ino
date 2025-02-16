#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>  // Include the SoftwareSerial library
#include "SuperMon.h"  // Include the file containing the HTML/CSS/JS for the web page

// WiFi credentials
const char* ssid = "IITP-AP";
const char* password = "iitp@1234";

// Declare the server
ESP8266WebServer server(80);

// Variables to hold sensor data
int flameStatus = 0;
int rainStatus = 0;
int intruderStatus = 0;

String generateJSON();  // Forward declaration for JSON generation function

// Create a SoftwareSerial object
SoftwareSerial softSerial(3,1);  // RX, TX pins for communication with Arduino

void setup() {
  Serial.begin(57600);  // For debugging
  softSerial.begin(9600);  // For communication with Arduino
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", PAGE_MAIN);
  });

  server.on("/json", []() {
    server.send(200, "application/json", generateJSON());
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Check if data is available on the software serial
  if (softSerial.available() > 0) {
    String data = softSerial.readStringUntil('\n');
    data.trim();  // Remove any leading or trailing whitespace

    // Debugging output to the default serial
    Serial.println("Received: " + data);

    // Parse the data
    if (data.startsWith("FLAME:")) {
      flameStatus = data.substring(6).toInt();
    } else if (data.startsWith("RAIN:")) {
      rainStatus = data.substring(5).toInt();
    } else if (data.startsWith("INTRUDER:")) {
      intruderStatus = data.substring(9).toInt();
    } else {
      Serial.println("Unknown data received");
    }
  }

  server.handleClient();
}

String generateJSON() {
  StaticJsonDocument<200> doc;

  doc["flame"] = flameStatus;
  doc["rain"] = rainStatus;
  doc["intruder"] = intruderStatus;

  String json;
  serializeJson(doc, json);
  return json;
}
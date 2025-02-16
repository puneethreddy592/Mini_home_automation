#include <Servo.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

Servo myservo;

int flameSensor = A0;
int irSensor = 2;
int rainSensor = A1;
int buzzer = 3;
bool flame = 0;
bool rain = 0;
bool intruder = 0;
int flameled = 8;

int ledPin = 13;
// SoftwareSerial mySerial(10, 11);  // RX, TX (use different pins, e.g., 10 and 11)

void setup() {
  pinMode(flameSensor, INPUT);
  pinMode(irSensor, INPUT);
  pinMode(rainSensor, INPUT);
  pinMode(flameled, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);

  myservo.attach(9);

  Serial.begin(9600);      // Changed to match mySerial baud rate
  // mySerial.begin(9600);

  // Serial.println("System Initialized");
}

void loop() {
  int flameValue = analogRead(flameSensor);
  int irValue = digitalRead(irSensor);
  int rainValue = analogRead(rainSensor);

  if (flameValue > 700) {
    flame = 1;
    digitalWrite(flameled, HIGH);
    digitalWrite(buzzer, LOW);
    // Serial.println("Fire detected!");
  } else {
    digitalWrite(flameled, LOW);
    digitalWrite(buzzer, HIGH);
    flame = 0;
  }

  if (rainValue > 30) {
    rain = 1;
    // Serial.println("Rain detected!");
  } else {
    rain = 0;
  }

  if (irValue == HIGH) {
    intruder = 1;
    // Serial.println("Intruder detected!");
  } else {
    intruder = 0;
  }

  if (flame || (rain && intruder)) {
    myservo.write(90);
  } else if (rain || intruder) {
    myservo.write(0);
  } else {
    myservo.write(90);
  }

  if (Serial.available()) {
    if (Serial.read() == 'j') {
      StaticJsonDocument<200> doc;
      doc["Flame"] = flame;
      doc["Rain"] = rain;
      doc["Intruder"] = intruder;
      serializeJsonPretty(doc, Serial);
      // mySerial.println();
      // Serial.println("JSON sent");
    }
  }


  delay(500);  // Wait for half a second before the next loop
}
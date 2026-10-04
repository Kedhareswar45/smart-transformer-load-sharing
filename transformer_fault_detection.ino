#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <WiFi.h>
#include <ThingSpeak.h>

// ---------------- WiFi ----------------
const char* ssid = "SRC 24G";
const char* password = "src@internet";
WiFiClient client;

// ---------------- ThingSpeak ----------------
unsigned long myChannelNumber = 683266;
const char* myWriteAPIKey = "5ZOYWIEG9OI55HMY";

// ---------------- I2C LCD ----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- Sensors ----------------
Adafruit_INA219 ina219;
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- ESP32 Pins ----------------
#define BUZZER 18
#define MOTOR  23

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();
  lcd.print("Connecting WiFi");

  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  digitalWrite(MOTOR, HIGH);

  dht.begin();

  // -------- WiFi --------
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  lcd.print("WiFi Connected");

  ThingSpeak.begin(client);

  // -------- INA219 --------
  if (!ina219.begin()) {
    Serial.println("INA219 not found");
  }

  delay(1500);
  lcd.clear();
}

// ---------------- Loop ----------------
void loop() {
  // -------- INA219 --------
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();

  if (busvoltage < 1.1) busvoltage = 0;
  if (current_mA < 10) current_mA = 0;

  // -------- Temperature --------
  int t = dht.readTemperature();

  // -------- LCD Display --------
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(busvoltage, 1);
  lcd.print(" I:");
  lcd.print(current_mA, 0);

  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(t);

  // -------- ThingSpeak Upload --------
  ThingSpeak.setField(1, busvoltage);
  ThingSpeak.setField(2, current_mA);
  ThingSpeak.setField(3, t);

  int xhttp = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (xhttp == 200) {
    Serial.println("ThingSpeak Update OK");
  } else {
    Serial.println("ThingSpeak Error");
  }

  delay(1500); // ThingSpeak minimum delay

  // -------- Alerts --------
  if (t > 35) {
    digitalWrite(BUZZER, HIGH);
    lcd.clear();
    lcd.print("OVER TEMPERATURE");
    delay(1000);
    digitalWrite(BUZZER, LOW);
  }

  if (current_mA > 100) {
    digitalWrite(MOTOR, LOW);
    digitalWrite(BUZZER, HIGH);
    lcd.clear();
    lcd.print("OVER LOAD");
    delay(1000);
    digitalWrite(BUZZER, LOW);
  }

  if (busvoltage < 5) {
    digitalWrite(BUZZER, HIGH);
    lcd.clear();
    lcd.print("LOW VOLTAGE");
    delay(1000);
    digitalWrite(BUZZER, LOW);
  }
}

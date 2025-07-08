#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pins
#define DHTPIN 4
#define DHTTYPE DHT22
#define RED_PIN 18
#define GREEN_PIN 23
#define BUZZER_PIN 27

// OLED-Konfiguration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sensor
DHT dht(DHTPIN, DHTTYPE);

// Setup
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // OLED initialisieren
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED nicht gefunden"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

// Hilfsfunktion: Ampelsteuerung
void setLED(bool red, bool green) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
}

// Hauptloop
void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Sensorfehler prüfen
  if (isnan(temp) || isnan(hum)) {
    Serial.println("Fehler beim Lesen vom DHT-Sensor!");
    return;
  }

  // Anzeige im Seriellen Monitor
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" °C, Feuchte: ");
  Serial.print(hum);
  Serial.println(" %");

  // OLED aktualisieren
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp);
  display.print(" C");
  display.setCursor(0, 10);
  display.print("Feuchte: ");
  display.print(hum);
  display.print(" %");

  String status = "";

  // Logik für Ampelfarben
  bool kritisch = (temp < 16 || temp > 30 || hum < 30 || hum > 70);
  bool grenzwertig = (!kritisch) && ((temp >= 16 && temp <= 19) || (temp >= 26 && temp <= 30)
                                    || (hum >= 30 && hum <= 39) || (hum >= 61 && hum <= 70));
  bool optimal = (!kritisch && !grenzwertig);

  if (optimal) {
    setLED(false, true);  // Grün
    status = "Optimal";
    noTone(BUZZER_PIN);
  } else if (grenzwertig) {
    setLED(true, true);   // Gelb (Rot + Grün)
    status = "Grenzwertig";
    noTone(BUZZER_PIN);
  } else {
    setLED(true, false);  // Rot
    status = "Kritisch!";
    tone(BUZZER_PIN, 2000, 500);  // 2kHz für 500 ms
  }

  display.setCursor(0, 30);
  display.print("Status: ");
  display.print(status);
  display.display();

  delay(2000);
}

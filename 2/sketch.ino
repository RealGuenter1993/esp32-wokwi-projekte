#include "DHT.h"           // DHT Bibliothek einbinden
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 15          // DHT22 Sensor Signal auf ESP32 Eingang 15
#define DHTTYPE DHT22      // Definition des Sensortyps: DHT 22 (AM2302), AM2321
#define SCREEN_WIDTH 128   // OLED display width, in pixels
#define SCREEN_HEIGHT 64   // OLED display height, in pixels

DHT dht(DHTPIN, DHTTYPE);  // DHT22 Sensor initialisieren

// Deklaration: SSD1306 über I2C angeschlossen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ------------------------------------ S E T U P ------------------------------------

void setup() {
  Serial.begin(115200);    // Baud Rate, für ESP32 115200
  Serial.println("-------------------- Test --------------------");
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 Zuordnung nicht möglich"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}
 void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) { // isnan Funktion - variable IS Not a Number
    Serial.println("DHT Sensor konnte nicht korrekt gelesen werden!");
    delay(1000);
    display.clearDisplay();
  }

  // Anzeige der Temperatur auf dem Display
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperatur: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(t);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  // Anzeige der Luftfeuchtigkeit auf dem Display
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Luftfeuchtigkeit: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h);
  display.print("%");

  display.display();
 }
```
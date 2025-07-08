#include "DHT.h"           // DHT Bibliothek einbinden

#define DHTPIN 15          // DHT22 Sensor Signal auf ESP32 Eingang 15
#define DHTTYPE DHT22      // Definition des Sensortyps: DHT 22

DHT dht(DHTPIN, DHTTYPE);  // DHT22 Sensor initialisieren

void setup() {
  Serial.begin(115200);  // Baudrate für ESP32
  Serial.println("DHT22 Test!"); // Testmeldung
  dht.begin();           // DHT Sensor starten
}

void loop() {
  delay(2000); // Wartezeit von 2 Sekunden zwischen den Messungen

  float h = dht.readHumidity();    // Luftfeuchtigkeit auslesen
  float t = dht.readTemperature(); // Temperatur auslesen

  if (isnan(h) || isnan(t)) { // Überprüfen, ob die Sensorwerte gültig sind
    Serial.println("DHT Sensor konnte nicht korrekt gelesen werden!");
  } else {
    Serial.print("Luftfeuchtigkeit: ");
    Serial.print(h);
    Serial.println(" %");

    Serial.print("Temperatur: ");
    Serial.print(t);
    Serial.println(" °C");
    Serial.println();
  }
}
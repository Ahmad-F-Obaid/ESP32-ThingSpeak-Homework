#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"

const int DHT_PIN = 15;
DHTesp dhtSensor;

const char* ssid = "Wokwi-GUEST";
const char* password = "";

unsigned long myChannelNumber = 3551840; 
const char* myWriteAPIKey = "J9FZD0IU8A7R5REV"; 

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Student ID: 12220492"); 
  
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temp = data.temperature;
  float hum = data.humidity;

  Serial.print("Temperature: "); Serial.print(temp); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");

  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, hum);
  
  int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (statusCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.print("Error sending data. Code: ");
    Serial.println(statusCode);
  }

  delay(20000); 
}

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Aakash 5G1";
const char* password = "aakash1434";
String apiKey = "TYQZH7SNLKJ0F3PE"; // ThingSpeak write API key

void setup() {
  Serial.begin(115200);      // debug
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 
  // RX=16 TX=17 (adjust pins to your wiring)
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");
}

void loop() {
  if (Serial2.available()) {
    String dataLine = Serial2.readStringUntil('\n');
    dataLine.trim();
    Serial.println("Received: " + dataLine);

    // Split CSV
    float pm25, mq2, mq8, mq9, mq135, temp, hum;
    int count = sscanf(dataLine.c_str(), "%f,%f,%f,%f,%f,%f,%f", 
                       &pm25, &mq2, &mq8, &mq9, &mq135, &temp, &hum);
    if (count == 7) {
      sendToThingSpeak(pm25, mq2, mq8, mq9, mq135, temp, hum);
    }
  }
  delay(16000); // ThingSpeak minimum 15s
}

void sendToThingSpeak(float pm25, float mq2, float mq8, float mq9, float mq135, float temp, float hum) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(pm25) +
                 "&field2=" + String(mq2) +
                 "&field3=" + String(mq8) +
                 "&field4=" + String(mq9) +
                 "&field5=" + String(mq135) +
                 "&field6=" + String(temp) +
                 "&field7=" + String(hum);

    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("ThingSpeak response: " + http.getString());
    } else {
      Serial.println("Error sending to ThingSpeak");
    }
    http.end();
  }
}
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "DHT.h"

// ---------- OLED setup ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- DHT setup ----------
#define DHTPIN 7           // DHT22 signal pin connected to pin 7 on UNO
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------- Analog sensor pins ----------
#define PM25_PIN   A0       // PM2.5 sensor analog pin
#define MQ2_PIN    A1       // MQ-2 sensor analog pin
#define MQ8_PIN    A2       // MQ-8 sensor analog pin
#define MQ9_PIN    A3       // MQ-9 sensor analog pin
#define MQ135_PIN  A4       // MQ-135 sensor analog pin

void setup() {
  Serial.begin(9600);   // Serial to ESP32 (9600 baud)

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // 0x3C is the typical I2C address for 128x64 OLED
    while (true) {
      // If OLED not found, blink Arduino onboard LED
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(500);
    }
  }
  display.clearDisplay();
  display.display();

  // Initialize DHT
  dht.begin();

  // Optional: show boot message
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Air Quality Monitor");
  display.println("Initializing...");
  display.display();
  delay(2000);
}

void loop() {
  // -------- Read sensors --------
  float pm25 = analogRead(PM25_PIN);   // 0–1023 on UNO
  float mq2 = analogRead(MQ2_PIN);
  float mq8 = analogRead(MQ8_PIN);
  float mq9 = analogRead(MQ9_PIN);
  float mq135 = analogRead(MQ135_PIN);

  float temp = dht.readTemperature();  // Celsius
  float hum = dht.readHumidity();      // Percent

  // Handle NaN from DHT sensor
  if (isnan(temp) || isnan(hum)) {
    temp = 0;
    hum = 0;
  }

  // -------- Send CSV to ESP32 --------
  Serial.print(pm25); Serial.print(",");
  Serial.print(mq2); Serial.print(",");
  Serial.print(mq8); Serial.print(",");
  Serial.print(mq9); Serial.print(",");
  Serial.print(mq135); Serial.print(",");
  Serial.print(temp); Serial.print(",");
  Serial.println(hum);

  // -------- Display on OLED --------
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("=== Air Quality ===");

  display.setCursor(0, 10);
  display.print("PM2.5: "); display.println(pm25);

  display.setCursor(0, 20);
  display.print("MQ2: "); display.println(mq2);

  display.setCursor(0, 30);
  display.print("MQ8: "); display.println(mq8);

  display.setCursor(0, 40);
  display.print("MQ9: "); display.println(mq9);

  display.setCursor(64, 10);
  display.print("MQ135: "); display.println(mq135);

  display.setCursor(64, 30);
  display.print("T: "); display.print(temp); display.println("C");

  display.setCursor(64, 40);
  display.print("H: "); display.print(hum); display.println("%");

  display.display();

  // -------- Delay --------
  delay(3000); // update every 3 seconds
}
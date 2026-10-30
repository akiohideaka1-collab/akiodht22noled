#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHT1_PIN 19
#define DHT2_PIN 16
#define DHTTYPE DHT22

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1

DHT dht1(DHT1_PIN, DHTTYPE);
DHT dht2(DHT2_PIN, DHTTYPE);

Adafruit_SSD1306 display(
  OLED_WIDTH,
  OLED_HEIGHT,
  &Wire,
  OLED_RESET
);

const char* statusSuhu(float suhu) {
  if (suhu < 20.0) {
    return "DINGIN";
  }
  else if (suhu > 30.0) {
    return "PANAS";
  }
  else {
    return "NORMAL";
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 14);

  if (!display.begin(
      SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED error!");
    while (true) {
      delay(100);
    }
  }

  dht1.begin();
  dht2.begin();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("DUAL DHT22");
  display.display();

  delay(2000);
}

void loop() {
  float suhu1 = dht1.readTemperature();
  float hum1 = dht1.readHumidity();

  float suhu2 = dht2.readTemperature();
  float hum2 = dht2.readHumidity();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // Judul
  display.setCursor(0, 0);
  display.println("MONITOR DUAL DHT22");

  display.drawLine(0, 10, 127, 10,
                   SSD1306_WHITE);

  // DHT22 #1
  display.setCursor(0, 13);
  display.println("DHT22 #1");

  display.setCursor(0, 22);

  if (!isnan(suhu1) && !isnan(hum1)) {
    display.print("Suhu: ");
    display.print(suhu1, 1);
    display.print(" C  ");

    display.print("Hum: ");
    display.print(hum1, 0);
    display.println("%");
  } else {
    display.println("Sensor 1 ERROR!");
  }

  // DHT22 #2
  display.setCursor(0, 32);
  display.println("DHT22 #2");

  display.setCursor(0, 41);

  if (!isnan(suhu2) && !isnan(hum2)) {
    display.print("Suhu: ");
    display.print(suhu2, 1);
    display.print(" C  ");

    display.print("Hum: ");
    display.print(hum2, 0);
    display.println("%");
  } else {
    display.println("Sensor 2 ERROR!");
  }

  // STATUS
  display.drawLine(0, 51, 127, 51,
                   SSD1306_WHITE);

  display.setCursor(0, 54);
  display.print("STATUS: ");

  if (!isnan(suhu1)) {
    display.print("#1 ");
    display.print(statusSuhu(suhu1));
  } else {
    display.print("#1 ERROR");
  }

  display.print(" ");

  if (!isnan(suhu2)) {
    display.print("#2 ");
    display.print(statusSuhu(suhu2));
  } else {
    display.print("#2 ERROR");
  }

  display.display();

  // Serial Monitor
  Serial.println("===== DUAL DHT22 =====");

  Serial.print("Sensor 1: ");
  Serial.print(suhu1);
  Serial.print(" C | ");
  Serial.print(hum1);
  Serial.println("%");

  Serial.print("Sensor 2: ");
  Serial.print(suhu2);
  Serial.print(" C | ");
  Serial.print(hum2);
  Serial.println("%");

  delay(2500);
}

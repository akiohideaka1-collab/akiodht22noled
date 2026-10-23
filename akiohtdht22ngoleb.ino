
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 19
#define DHTTYPE DHT22

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(
  OLED_WIDTH,
  OLED_HEIGHT,
  &Wire,
  OLED_RESET
);

void setup() {
  Serial.begin(115200);

  // SDA = GPIO 25, SCL = GPIO 33
  Wire.begin(25, 33);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED tidak terdeteksi!");
    while (true) {
      delay(100);
    }
  }

  dht.begin();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("ESP32 + DHT22");
  display.setCursor(10, 35);
  display.println("OLED siap!");
  display.display();

  Serial.println("=== MONITOR SUHU ESP32 ===");
  Serial.println("Sensor DHT22 siap!");

  delay(2000);
}

void loop() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (isnan(suhu) || isnan(kelembapan)) {
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Sensor DHT22 error");

    Serial.println("ERROR: Periksa kabel DHT22!");
  } else {
    // Menentukan kondisi suhu
    String kondisi;

    if (suhu > 30.0) {
      kondisi = "PANAS";
    } else if (suhu < 20.0) {
      kondisi = "DINGIN";
    } else {
      kondisi = "NORMAL";
    }

    // Tampilan judul OLED
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.println("MONITOR SUHU");

    display.drawLine(
      0, 12, 127, 12, SSD1306_WHITE
    );

    // Tampilan suhu
    display.setCursor(0, 16);
    display.println("Suhu:");

    display.setTextSize(2);
    display.setCursor(0, 26);
    display.print(suhu, 1);
    display.print(" C");

    // Tampilan kondisi suhu
    display.setTextSize(1);
    display.setCursor(0, 46);
    display.print("Kondisi: ");
    display.println(kondisi);

    // Tampilan kelembapan
    display.setCursor(0, 56);
    display.print("RH: ");
    display.print(kelembapan, 1);
    display.print("%");

    // Informasi pada Serial Monitor
    Serial.println("------------------------");
    Serial.print("Suhu        : ");
    Serial.print(suhu, 1);
    Serial.println(" C");

    Serial.print("Kelembapan  : ");
    Serial.print(kelembapan, 1);
    Serial.println(" %");

    Serial.print("Kondisi     : ");
    Serial.println(kondisi);
  }

  display.display();
  delay(2000);
}

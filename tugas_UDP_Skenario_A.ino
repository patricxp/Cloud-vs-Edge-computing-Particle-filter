#include <WiFi.h>
#include <WiFiUdp.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

// ===== WIFI =====
const char* ssid = "Sekolah Robot Indonesia";
const char* password = "sadapsadap";

// ===== UDP =====
WiFiUDP udp;
const char* IP_RPI = "192.168.0.26"; // GANTI
const int PORT = 5000;

// ===== MPU =====
MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(115200);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  udp.begin(1234);
}

void loop() {
  mpu6050.update();

  float angleZ = mpu6050.getAngleZ();

  // kirim RAW data
  udp.beginPacket(IP_RPI, PORT);
  udp.print(angleZ, 4);
  udp.endPacket();

  Serial.println(angleZ);

  delay(50); // 20 Hz (cukup untuk testing)
}
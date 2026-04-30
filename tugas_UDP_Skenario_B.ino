#include <WiFi.h>
#include <WiFiUdp.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <math.h>

// ===== WIFI =====
const char* ssid = "Sekolah Robot Indonesia";
const char* password = "sadapsadap";

// ===== UDP =====
WiFiUDP udp;
const char* IP_RPI = "192.168.0.26";
const int PORT = 5000;

// ===== MPU =====
MPU6050 mpu6050(Wire);

// ===== PARTICLE FILTER =====
#define N 1000

float particles[N];
float weights[N];

void initParticles(float initVal) {
  for (int i = 0; i < N; i++) {
    particles[i] = initVal + random(-20, 20) * 0.1;
    weights[i] = 1.0 / N;
  }
}

float particleFilter(float measurement) {
  float sumW = 0;

  for (int i = 0; i < N; i++) {
    // Prediction (adaptasi ESP32)
    particles[i] += random(-200, 200) * 0.1;

    float diff = particles[i] - measurement;

    // Update weight (adaptasi ESP32)
    weights[i] = exp(-0.005 * diff * diff);

    if (weights[i] < 1e-6) weights[i] = 1e-6;

    sumW += weights[i];
  }

  // Normalisasi (ANTI NaN)
  if (sumW == 0 || isnan(sumW)) {
    for (int i = 0; i < N; i++) {
      weights[i] = 1.0 / N;
    }
  } else {
    for (int i = 0; i < N; i++) {
      weights[i] /= sumW;
    }
  }

  // Estimasi
  float estimate = 0;
  for (int i = 0; i < N; i++) {
    estimate += particles[i] * weights[i];
  }

  return estimate;
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  delay(1000);

  // Ambil nilai awal sensor
  mpu6050.update();
  float initVal = mpu6050.getAngleZ();

  initParticles(initVal);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  udp.begin(1234);
}

void loop() {
  mpu6050.update();

  float raw = mpu6050.getAngleZ();

  unsigned long start = micros();
  float filtered = particleFilter(raw);
  unsigned long duration = micros() - start;

  // FAIL-SAFE (SETELAH filter dihitung)
  if (abs(filtered - raw) > 50) {
    Serial.println("Reinit particles!");
    initParticles(raw);
  }

  // Kirim: raw, filtered, time(us)
  udp.beginPacket(IP_RPI, PORT);
  udp.printf("%.4f,%.4f,%lu\n", raw, filtered, duration);
  udp.endPacket();

  Serial.print("RAW: ");
  Serial.print(raw);
  Serial.print(" | FILTERED: ");
  Serial.print(filtered);
  Serial.print(" | Time(us): ");
  Serial.println(duration);

  delay(50);
}
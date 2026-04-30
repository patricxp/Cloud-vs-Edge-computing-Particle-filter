# Cloud vs Edge Computing - Particle Filter

## 📌 Deskripsi

Proyek ini bertujuan untuk membandingkan performa **Particle Filter** pada dua pendekatan komputasi, yaitu:

* **Cloud/Server-Side Filtering** menggunakan Raspberry Pi
* **Edge/Client-Side Filtering** menggunakan ESP32

Pengujian dilakukan dengan memanfaatkan data dari sensor IMU yang dikirim melalui protokol UDP, kemudian dianalisis dari sisi waktu pemrosesan, stabilitas, dan kualitas hasil filtering.

---

## ⚙️ Arsitektur Sistem

### 🔹 Skenario A - Cloud (Server-Side)

ESP32 → kirim raw data (UDP) → Raspberry Pi → filtering → plotting

### 🔹 Skenario B - Edge (Client-Side)

ESP32 → filtering → kirim data hasil (UDP) → Raspberry Pi → plotting

---

## 🧠 Metode

* Algoritma: Particle Filter
* Jumlah partikel: 1000
* Sensor: IMU
* Komunikasi: UDP
* Platform:

  * ESP32 (Edge Computing)
  * Raspberry Pi (Cloud/Server)

---

## 📊 Hasil Utama

* **Raspberry Pi**

  * Processing cepat (< 1 ms)
  * Terdapat jitter (tidak stabil)

* **ESP32**

  * Processing lebih lambat (~13 ms)
  * Sangat stabil (deterministik)

* **Kualitas Filter**

  * Hasil filtering pada ESP32 hampir identik dengan Raspberry Pi

---

## ⚠️ Catatan Penting

Pengujian ini belum mencakup **end-to-end latency**, sehingga belum dapat disimpulkan secara pasti skenario mana yang memiliki total delay lebih rendah.

---

## 🎯 Kesimpulan

* Edge computing (ESP32) lebih unggul dalam stabilitas waktu eksekusi
* Cloud computing (Raspberry Pi) unggul dalam kecepatan komputasi
* Particle Filter dapat diimplementasikan secara efektif pada perangkat edge tanpa penurunan akurasi signifikan

---

## 🚀 Pengembangan Selanjutnya

* Pengukuran end-to-end latency
* Optimasi jumlah partikel
* Implementasi pada sistem kontrol real-time (robotika)

---

## 🔗 Link

* YouTube: -
* GitHub: -

---

## 👤 Author

Muhammad Najib

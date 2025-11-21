# CloudVision – Standalone ESP32-CAM Face Recognition Attendance System
![License: CC BY-NC-ND 4.0](https://img.shields.io/badge/License-CC--BY--NC--ND%204.0-blue.svg)
![Protected](https://img.shields.io/badge/Protected-Yes-brightgreen)


A **cloud-powered, standalone attendance system** using **ESP32-CAM**, **Flask face-recognition server on AWS EC2**, and **SD-card logging**.  
This system captures a face image through ESP32-CAM, sends it to a cloud server for recognition, and stores attendance locally on an SD card — all without any laptop dependency.

---

## 🚀 Project Highlights

- **Standalone Operation** – Fully functional without laptop/Raspberry Pi  
- **Cloud-based Face Recognition** via Flask API hosted on AWS EC2  
- **ESP32-CAM Image Capture** triggered by a physical button  
- **Local SD Card Attendance Logging** with timestamps  
- **LED Status Indicators** for capture, success & failure  
- **Low-cost & Scalable** design ideal for colleges, offices & schools  

---

## 🧠 System Architecture

[ Person in front of camera ]
          |
          v
   +----------------+
   |   Push Button  |
   +----------------+
          |
          v
   +----------------+
   |   ESP32-CAM    |
   | Captures Image |
   | Sends to API   |
   +----------------+
          |
   HTTP POST Request
          |
          v
+--------------------------------+
|     Flask Server on AWS EC2    |
|  - Face Detection (OpenCV)     |
|  - Face Recognition            |
|  - Returns Identified Name     |
+--------------------------------+
          |
     JSON Response
          |
          v
   +----------------+
   |   ESP32-CAM    |
   | - Parse Result |
   | - Log to SD    |
   | - LED Status   |
   +----------------+
          |
          v
 [ Attendance Stored ]

---

## 🎯 Features

### ✔ 1. **Cloud-based Face Recognition**
ESP32-CAM sends captured images to a Flask server hosted on AWS EC2.  
The server performs:
- Face detection  
- Feature extraction  
- Recognition  
- Sends recognized name back to ESP32

---

### ✔ 2. **Switch-Based Trigger**
Attendance is taken **only when the user presses the button**, preventing unwanted continuous scanning.

---

### ✔ 3. **Local Attendance Storage (SD Card)**
Data format:

---

### ✔ 4. **LED Indicators**
- **Red LED** – System ready  
- **Blue LED** – Image captured  
- **Green LED** – Attendance recorded successfully  

---

### ✔ 5. **Scalability**
- More faces can be added  
- Can integrate with web dashboard  
- Can connect with mobile apps  
- Cloud backend can be enhanced anytime  

---

## 🧰 Hardware Requirements

| Component | Description |
|----------|-------------|
| ESP32-CAM AI Thinker | Main microcontroller + camera |
| MicroSD Card | Store attendance logs |
| Push Button | Trigger for attendance capture |
| LEDs + Resistors | Visual status |
| Jumper Wires | Connections |
| Wi-Fi Network | For cloud communication |

---

## 🧰 Software Requirements

| Software | Purpose |
|----------|---------|
| Arduino IDE | ESP32 firmware |
| Flask | API backend |
| OpenCV | Face detection |
| face_recognition | Face recognition |
| Python 3.9+ | Server-side environment |
| AWS EC2 | Cloud hosting |

---

## ⚙ Installation & Setup

### 🔹 1. **ESP32-CAM Setup (Arduino IDE)**
Add ESP32 boards manager:
https://dl.espressif.com/dl/package_esp32_index.json

Install required libraries:
- WiFi.h  
- FS.h  
- SD_MMC.h  

Update your Wi-Fi and server URL inside the `.ino` file:
```cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
String serverURL = "http://YOUR-EC2-IP:5000/recognize";
```

### 🔹 2. **Flask Server Setup (AWS EC2)**

On your EC2 Ubuntu instance:

sudo apt update
sudo apt install python3-pip
pip3 install flask opencv-python face_recognition numpy

Run the Flask server:

python3 app.py

Make sure port 5000 is allowed in AWS Security Groups.

## 📁 Project Structure

/CloudVision-ESP32-Standalone
├── hardware/
│   ├── esp32_cam.ino
│   └── wiring_diagram.png
├── server/
│   ├── app.py
│   ├── face_encodings/
│   ├── requirements.txt
├── docs/
│   ├── Final_Project_Report.pdf
│   ├── architecture.png
├── examples/
│   ├── sample_capture.jpg
└── README.md

## 🔄 Working Process

1. User presses the button  
2. ESP32-CAM captures an image  
3. Image is sent to AWS EC2 Flask Server  
4. Server detects & recognizes face  
5. Sends back name or “Unknown”  
6. ESP32 logs attendance in SD card  
7. LED shows attendance success  
8. System resets for next user

## 📊 Performance

- Accuracy: ~92%  
- Response Time: 1–2 seconds  
- Detection Distance: 1–3 meters  
- Storage: Unlimited (SD Card)  

## 🔮 Future Enhancements

- Add LCD/OLED Display  
- Web Dashboard  
- Mobile App  
- QR/OTP based authentication  
- Deep Learning models  
- Voice feedback  
## 👩‍💻 Author

**Titli Ghosh**  
RCCIIT  
Electrical Engineering, 2025

## 📜 License
MIT License

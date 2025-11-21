#define BLYNK_TEMPLATE_ID "TMPL3XFvC1tQc"
#define BLYNK_TEMPLATE_NAME "Attendance System"
#define BLYNK_AUTH_TOKEN "lYz401CkKDrofRu0nsGicdTTklv5Eu5t"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "esp_camera.h"
#include <HTTPClient.h>
#include "FS.h"
#include "SD_MMC.h"

// ===== Blynk Config =====
char auth[] = BLYNK_AUTH_TOKEN;

// ===== WiFi Credentials =====
const char* ssid = "Titlighosh";
const char* password = "rinaghosh1.";

// ===== Flask Server URL =====
const char* serverUrl = "http://13.60.202.5:5000/recognize";

// ===== Switch Pin =====
#define SWITCH_PIN 13

// ===== Camera Pins (AI Thinker) =====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x\n", err);
    return;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(SWITCH_PIN, INPUT_PULLUP);  // Use INPUT if using external pull-down resistor

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
  delay(2000);

  Blynk.begin(auth, ssid, password, "blr1.blynk.cloud", 80);

  startCamera();

  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
  } else {
    Serial.println("SD Card mounted");
  }
}

void loop() {
  Blynk.run();

  // Read switch (LOW when pressed using INPUT_PULLUP)
  if (digitalRead(SWITCH_PIN) == LOW) {
    Serial.println("Switch pressed, capturing image...");
    delay(300);  // Debounce

    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/octet-stream");
    int httpResponseCode = http.POST(fb->buf, fb->len);
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response: " + response);

      if (response.indexOf("Unknown") == -1 && response.indexOf("names") != -1) {
        int startIndex = response.indexOf("[\"") + 2;
        int endIndex = response.indexOf("\"]");
        String name = response.substring(startIndex, endIndex);

        File file = SD_MMC.open("/attendance.txt", FILE_APPEND);
        if (file) {
          String log = name + ", " + String(millis()) + "\n";
          file.print(log);
          file.close();
          Serial.println("Attendance logged: " + log);
        } else {
          Serial.println("Failed to write to SD card");
        }

        Blynk.virtualWrite(V0, name);
      }
    } else {
      Serial.println("POST failed: " + http.errorToString(httpResponseCode));
    }

    http.end();
    esp_camera_fb_return(fb);

    // Optional: Add delay to avoid multiple triggers from a single press
    delay(2000);
  }
}

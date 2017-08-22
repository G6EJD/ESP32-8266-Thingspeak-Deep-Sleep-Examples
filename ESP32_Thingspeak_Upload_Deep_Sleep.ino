#include <WiFi.h>
#include "esp_deep_sleep.h" //Library needed for ESP32 Sleep Functions
WiFiClient client; // wifi client object

const char *ssid     = "your_SSID";
const char *password = "your_PASSWORD";

char ThingSpeakAddress[] = "api.thingspeak.com"; // Thingspeak address
String api_key = "P1CS1NBUJXRAUSPS";             // Thingspeak API WRITE Key for your channel
const int UpdateInterval = 15 * 60 * 1000000;  // e.g. 15 * 60 * 1000000; for a 15-Min update interval (15-mins x 60-secs * 1000000uS)

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Start WiFi");
  while (WiFi.status() != WL_CONNECTED ) {Serial.print("."); delay(500); }

  float temp = 20.5; // 3 example variables, ideally supplied by a sensor, see my examples for the BMP180, BME280 or DS18B20
  float humi = 60;
  float pres = 1015.6;

  UpdateThingSpeak("field1="+String(temp)+"&field2="+String(humi)+"&field3="+String(pres)); //Send the data as text
  esp_deep_sleep_enable_timer_wakeup(UpdateInterval);
  Serial.println("Going to sleep now...");
  esp_deep_sleep_start();
}

void loop() {
  //Do nothing as it will never get here!
}

void UpdateThingSpeak(String DataForUpload) {
  WiFiClient client;
  if (!client.connect(ThingSpeakAddress, 80)) {
     Serial.println("connection failed");
     return;
  }
  else
  {
    Serial.println(DataForUpload);
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(DataForUpload.length());
    client.print("\n\n");
    client.print(DataForUpload);
  }
  client.stop();
}


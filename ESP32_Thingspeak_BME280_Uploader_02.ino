// Weather station V2 board sample

#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_BME280.h>

const char* ssid     = "your_SSID";
const char* password = "your_PASSWORD";

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String    writeAPIKey = "32CCT6D9GBIDV0NW";   // Get the key for your channel to approve writing
const int UpdateThingSpeakInterval = 10 * 60; // e.g. 10 * 60 for a 10-Min update interval (10-mins x 60-secs)
#define pressure_offset 3.95                  // Adjusts BME280 pressure value for my location at 40M asl
//#define sda 4
//#define scl 5
int timer;

Adafruit_BME280 bme;
  
void setup() {
  Serial.begin(115200);
  timer = millis();
  Serial.println("\n   Start:" + String(timer));
  WiFi.begin(ssid, password);
  Serial.println("Time to connect to WiFI:" + String(millis() - timer));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Wire.begin(); // Defaults but can use Wire.begin(SDA,SCL) e.g. Wire.begin(22,21) or any other pins for the I2C bus
//  if (!bme.begin(0x76)) Serial.println("Could not find a sensor, check wiring!");
//  else
//  {
//    Serial.println("Found a sensor continuing");
//    while (isnan(bme.readTemperature())) { Serial.println(bme.readTemperature()); } // The serial print of sensor value is not needed, but helps in debugging
//  }
  delay(500);
  float current_temperature = 22; //bme.readTemperature();
  float current_humidity    = 51; //bme.readHumidity();
  float current_pressure    = 1001;// bme.readPressure()/100.0F + pressure_offset;
  Serial.println(String(current_temperature,1)+"'c"); 
  Serial.println(String(current_humidity,0)+"%"); 
  Serial.println(String(current_pressure,1)+"hPa"); 
  UpdateThingSpeak(current_temperature, current_humidity, current_pressure);
  Serial.println("Finished:" + String(millis() - timer));
  Serial.println("Going to sleep...");
  esp_sleep_enable_timer_wakeup(UpdateThingSpeakInterval * 1000000LL);
  esp_deep_sleep_start();      // Sleep for e.g. 30 minutes
}

void loop() {
}

void UpdateThingSpeak(float current_temperature, float current_humidity, float current_pressure) {
  WiFiClient client;
  //GET http://api.thingspeak.com/update?api_key=32CCT6D9GBIDV0NW&field1=22&field2=23&field3=1028
  String url = "GET /update?api_key=" + writeAPIKey + "&field1=" + String(current_temperature,1) +
                                                      "&field2=" + String(current_humidity,1)    +
                                                      "&field3=" + String(current_pressure,1)    +
                                                      " HTTP/1.1";
  if (client.connect(thingSpeakAddress, 80))
  {
    client.println(url);
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close\n");
    Serial.println("Upload of data complete");
    if (client.connected()) Serial.println("Connecting to ThingSpeak...");
    else                    Serial.println("Connection to ThingSpeak failed");
  }
  else
  {
    Serial.println("Connection to ThingSpeak Failed");
  }
  delay(1000);    // Essential to enable upload to complete
  client.stop();  // close the connection
}

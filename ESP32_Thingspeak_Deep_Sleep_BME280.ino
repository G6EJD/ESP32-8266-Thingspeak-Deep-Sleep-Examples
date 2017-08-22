/* Last update: 22-Aug-17 with improved sensor startup checks  
 * ESP32 and a BME280 sensor that updates a Thingspeak channel, then goes to Deep Sleep
 * During Deep Sleep the ESP32 consumes ~5uA
 * The 'MIT License (MIT) Copyright (c) 2016 by David Bird'. Permission is hereby
 * granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including 
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * The above copyright ('as annotated') notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software and where the software use is visible
 * to an end-user.    
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * See more at http://dsbird.org.uk 
 * Also please refer to Adafruit for their Licences
*/ 
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "esp_deep_sleep.h" //Library needed for ESP32 Sleep Functions
WiFiClient client; // wifi client object

const char *ssid     = "your_SSID";
const char *password = "your_PASSWORD";

char ThingSpeakAddress[] = "api.thingspeak.com"; // Thingspeak address
String api_key = "P1CS1NBUJXRAUSPS";             // Thingspeak API WRITE Key for your channel
const int UpdateInterval = 0.33 * 60 * 1000000;  // e.g. 0.33 * 60 * 1000000; //20-Sec update interval for development tests, to fast for practical purposes and Thingspeak!
//const int UpdateInterval = 15 * 60 * 1000000;  // e.g. 15 * 60 * 1000000; for a 15-Min update interval (15-mins x 60-secs * 1000000uS)

#define pressure_offset 3.9 // Compensates for this location being 40M asl  
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Start WiFi");
  while (WiFi.status() != WL_CONNECTED ) {Serial.print("."); delay(500); }
  Wire.begin(17,16); // (sda,scl) 
  // These pins are for ESP32 EH-ET LIVE board as the sensor is VCC-Gnd-SCL-SDA and the Wemos D1 variant of the ESP32 has Vcc-Gnd-SCL_SDA so just plugs in, no wiring
  
  if (!bme.begin()) {
    Serial.println("Could not find a sensor, check wiring!");
  } 
  else
  {
    Serial.println("Found a sensor continuing");
    while (isnan(bme.readPressure())) {
      Serial.println(bme.readPressure());
    }
  }

  float temperature = bme.readTemperature(); // 3 example variables, ideally supplied by a sensor, see my examples for the BMP180, BME280 or DS18B20
  float humidity    = bme.readHumidity();
  float pressure    = bme.readPressure() / 100.0F + pressure_offset;

  UpdateThingSpeak("field1="+String(temperature)+"&field2="+String(humidity)+"&field3="+String(pressure)); //Send the data as text
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


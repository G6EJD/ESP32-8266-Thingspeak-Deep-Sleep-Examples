#include <ESP8266WiFi.h>
WiFiServer server(80);
// *************************************************************************************************************
// **** NOTE: The ESP8266 only supports deep sleep mode by connecting GPIO 16 to Reset
// Wiring: ESP8266 16 -> ESP8266 RST (or D0 for GPIO16 on most ESP8266 Dev Boards
// 
// !!!!! REMEMBER TO REMOVE THE GPIO16 to RST link for (re) programming, otherwise it remains in a RESET state..
//
// *************************************************************************************************************
const char *ssid     = "your_SSID";
const char *password = "your_PASSWORD";

char ThingSpeakAddress[] = "api.thingspeak.com"; // ThingSpeak address
String       writeAPIKey = "P1CS1NBUJXRAUSPS";   // Thingspeak Write Key to approve channel writing
const int UpdateInterval = 0.33 * 60 * 1000000;  // e.g. 15 * 60 * 1000000; for a 15-Min update interval (15-mins x 60-secs * 1000000uS)

void setup() {
  Serial.begin(115200); // Or use 74480 to see Boot messages too
  WiFi.begin(ssid, password);
  Serial.println("Start WiFi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print(".");} // Wait for a WiFi connection
  Serial.println();
  server.begin();

  float temp  = 20.1; // 3 example variables, ideally supplied by a sensor, see examples for the BMP180, BME280 or DS18B20
  float humi  = 65;
  float pres  = 1010.5;
  
  UpdateThingSpeak("field1="+String(temp)+"&field2="+String(humi)+"&field3="+String(pres)); // Send the data as text
  Serial.println("Going to sleep now...");
  ESP.deepSleep(UpdateInterval, WAKE_RF_DEFAULT); // Sleep for the time set by 'UpdateInterval'
}

void loop(){
  // Do nothing as it will never get here!
}

void UpdateThingSpeak(String DataForUpload){ // takes ~ 2.5Secs to complete an update, from power-up to entering sleep
  Serial.println(DataForUpload); // Diagnostic print so you can see what data is being sent to the server
  WiFiClient client = server.available();
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
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(DataForUpload.length());
    client.print("\n\n");
    client.print(DataForUpload);
  }
  client.stop();  // close the connection
}


# ESP32-8266-Thingspeak-Deep-Sleep-Examples

Two versions that demonstrate how to upload data to Thingspeak.com and then to enter Deep Sleep.

The ESP8266 consumes around 75mA in normal operation (WiFI on) and in Deep Sleep about 19uA. The ESP8266 takes ~ 2.5secs to upload data.

The ESP32 consumes around 75mA in normal operation (WiFi on) and in Deep Sleep about 5uA. The ESP32 takes ~ 2.5secs to upload data.

Battery Life calculations. Assume a 3.6v (3x1.2 NiMHd) battery of capacity 2200mAH and a time-on duration of 2.5-secs and
then 9mins 57.5secs (595-secs) of sleep (6x per hour):

For the ESP8266: Total mAH consumption per hour
 (ON time) = 6 x 75mA * 2.5 / 3600 = 0.313mAH
 (DS time) = 6 x 19uA * 597.5 / 3600 = 0.114mAH
 Total time in hours on battery = 2200/(0.0313+0.114) = 5159Hrs
 Total days on battery = 5159/24 = 214 Days

For the ESP32: Total mAH consumption per hour
 (ON time) = 6 x 75mA * 2.5 / 3600 = 0.313mAH
 (DS time) = 6 x  5uA * 597.5 / 3600 = 0.03mAH
 Total time in hours on battery = 2200/(0.313+0.03) = 6423Hrs
 Total days on battery = 7092/24 = 267 Days
 
 3-samples/hour extends battery life to: ESP8266 = 429Days or 1.2years and ESP32 = 535Days  or 1.5years
 
        
        
        
        
        
        
        
        
        
        
        

#include <DHT.h>  // Including library for dht
#include<MQUnifiedsensor.h>
#include <ESP8266WiFi.h>
 
String apiKey = "M4EHKZWKFFXHNH9G";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Believer";     // replace with your wifi ssid and wpa2 key
const char *pass =  "vigna7257";
const char* server = "api.thingspeak.com";
float R0 = 15000;
#define DHTPIN 0   //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);

WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
     
      float sensorval = 0;
      float sensorvolt = 0;
      float RS_gas;
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }


                 for(int i = 0; i < 100; i++)
                 {
                    sensorval += analogRead(A0);
                 }

                 sensorval /= 100; // avg
                 sensorvolt = sensorval/1024*5.0;
                 RS_gas = (5.0-sensorvolt)/sensorvolt;
                float  ratio = RS_gas/R0;
                float  x = 1000*ratio;
                 float LPG_PPM = pow(x,-1.431);//LPG PPM
               
  
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;                 
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "&field3=";
                             postStr += String(LPG_PPM);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.print(" Conc. of LPG in PPM: ");
                             Serial.print(LPG_PPM);
                             Serial.println("%. Send to Thingspeak.");
                        } 
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}

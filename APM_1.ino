#include <DHT.h>  // Including library for dht
#include<MQUnifiedsensor.h>
#include <ESP8266WiFi.h>

#define board "ESP8266"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm

MQUnifiedsensor MQ6(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type);
 
String apiKey = "M4EHKZWKFFXHNH9G";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Believer";     // replace with your wifi ssid and wpa2 key
const char *pass =  "vigna7257";
const char* server = "api.thingspeak.com";
#define DHTPIN 0   //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);

WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
       MQ6.setRegressionMethod(1);
       MQ6.init();
       MQ6.setRL(10);
       Serial.print("Calibrating please wait.");
       float calcR0 = 0;
       for(int i = 1; i<=10; i ++)
       {
           MQ6.update(); // Update data, the arduino will be read the voltage on the analog pin
           calcR0 += MQ6.calibrate(RatioMQ135CleanAir);
           Serial.print(".");
       }
       MQ135.setR0(calcR0/10);
       Serial.println("  done!.");
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
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      MQ6.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ6.setA(605.18); MQ6.setB(-3.937); // Configurate the ecuation values to get CO concentration
  float H2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ6.setA(77.255); MQ6.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
  float LPG = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ6.setA(110.47); MQ6.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  float CH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }


              
               
  
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;                 
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "&field3=";
                             postStr += String(H2);
                             postStr += "&field4=";
                             postStr += String(LPG);
                             postStr += "&field5=";
                             postStr += String(CH4);
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
                             Serial.print(" Conc. of H2 in PPM: ");
                             Serial.print(H2);
                             Serial.print(" Conc. of LPG in PPM: ");
                             Serial.print(LPG);
                             Serial.print(" Conc. of CH4 in PPM: ");
                             Serial.print(CH4);
                             Serial.println("%. Send to Thingspeak.");
                        } 
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(20000);
}

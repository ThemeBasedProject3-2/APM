#include <DHT.h>  // Including library for dht
#include<MQUnifiedsensor.h>
#include <ESP8266WiFi.h>

#define board "ESP8266"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type1 "MQ-6" 
#define type2 "MQ-7"
#define type3 "MQ-135"
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6 //RS / R0 = 3.6 ppm
#define RatioMQ6CleanAir 10 //RS / R0 = 10 ppm
#define RatioMQ7CleanAir 27.5

// Defining the threshold values of the gases

#define H2T 50
#define LPGT 2000
#define COT 50
#define NH3T 25
#define CO2T 5000
#define TOULENE_T 100
//=================================================================================//



MQUnifiedsensor MQ6(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type1);
MQUnifiedsensor MQ7(board, Voltage_Resolution, ADC_Bit_Resolution,pin,type2);
MQUnifiedsensor MQ135(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type3);

 
String apiKey = "M4EHKZWKFFXHNH9G";     //  Enter your Write API key from ThingSpeak
String ApiKey = "FA3B1CES10VIG330"; 
const char *ssid =  "Believer";     // replace with your wifi ssid and wpa2 key
const char *pass =  "vigna7257";
const char* server = "api.thingspeak.com";
#define DHTPIN 2  //pin where the dht11 is connected
 
DHT dht(DHTPIN, DHT11);

WiFiClient client;

void calibrateMQ6()
{
       Serial.print("Calibrating MQ6 please  wait.");
       float calcR0 = 0;
       for(int i = 1; i<=10; i ++)
       {
           MQ6.update(); // Update data, the arduino will be read the voltage on the analog pin
           calcR0 += MQ6.calibrate(RatioMQ6CleanAir);
           Serial.print(".");
       }
       MQ6.setR0(calcR0/10);
 
 Serial.println("  done!.");
 if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
 if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
 
 
 
  MQ6.serialDebug(true);
 
 }
 
void calibrateMQ7()
{
       Serial.print("Calibrating please MQ7 wait.");
       float calcR0 = 0;
       for(int i = 1; i<=10; i ++)
       {
           MQ7.update(); // Update data, the arduino will be read the voltage on the analog pin
           calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
           Serial.print(".");
       }
       MQ7.setR0(calcR0/10);   
      Serial.println("  done!.");
 
    
     if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
     if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
 
 
}

void calibrateMQ135()
{
       Serial.print("Calibrating please MQ135 wait.");
       float calcR0 = 0;
       for(int i = 1; i<=10; i ++)
       {
           MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
           calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
           Serial.print(".");
       }
       MQ135.setR0(calcR0/10);   
      Serial.println("  done!.");
 
 
     if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
     if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
 
 
 
 
}

void initMQ6()
{
       MQ6.setRegressionMethod(1);
       MQ6.init();
       MQ6.setRL(10);
       calibrateMQ6();
 
}

void initMQ7()
{
       MQ7.setRegressionMethod(1);
       MQ7.init();
       MQ7.setRL(10);
       calibrateMQ7();
      
 
}

void initMQ135()
{
       MQ135.setRegressionMethod(1);
       MQ135.init();
       MQ135.setRL(10);
       calibrateMQ135();
      
}

void MakeLow()
{
       digitalWrite(D1, LOW);
       digitalWrite(D2, LOW);
       digitalWrite(D3, LOW);
 
}

void setup() 
{
       Serial.begin(115200);
       delay(10);
       dht.begin();
       initMQ6();
       initMQ7();
       initMQ135();
       
       pinMode(D1, OUTPUT);   
       pinMode(D2, OUTPUT);
       pinMode(D3, OUTPUT);  
       
       MakeLow();
  
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
      MQ7.update();
      MQ135.update();
      MakeLow();
  digitalWrite(D1, HIGH);
  MQ6.setA(88158); MQ6.setB(-3.597); // Configurate the ecuation values to get H2 concentration
  float H2 = MQ6.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup


  MQ6.setA(1009.2); MQ6.setB(-2.35); // Configurate the ecuation values to get LPG concentration
  float LPG = MQ6.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
  digitalWrite(D2, HIGH); 
  MQ7.setA(99.042); MQ7.setB(-1.518);  //Configure the ecuation values to get CO concentration
  float CO = MQ7.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  
 
 digitalWrite(D3, HIGH);
  MQ135.setA(102.2); MQ135.setB(-2.473);
  float  NH3 = MQ135.readSensor();
 
   MQ135.setA(44.947); MQ135.setB(-3.445);
   float  Toulene = MQ135.readSensor();
   
   MQ135.setA(110.47); MQ135.setB(-2.862);
   float  CO2 = MQ135.readSensor();
   
      
              if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                     delay(2500);
                      return;
                 }


              
               
  
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                          //Channel 1
                            
                             String postStr = apiKey;                 
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "&field3=";
                             postStr += String(H2);
                             postStr += "&field4=";
                             postStr += String(H2T);
                             postStr += "&field5=";
                             postStr += String(LPG);
                             postStr += "&field6=";
                             postStr += String(LPGT);
                             postStr += "&field7=";
                             postStr += String(CO);
                             postStr += "&field8=";
                             postStr += String(COT);
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
                          
                          //Channel 2
                             postStr = "";
                             postStr = ApiKey;
                             postStr += "=&field1=";
                             postStr += String(NH3);
                             postStr += "&field2=";
                             postStr += String(NH3T);
                             postStr += "&field3=";
                             postStr += String(Toulene);
                             postStr += "&field4=";
                             postStr += String(TOULENE_T);
                             postStr += "&field5=";
                             postStr += String(CO2 + 400);
                             postStr += "&field6=";
                             postStr += String(CO2T);
                             postStr += "\r\n\r\n";
                          
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+ApiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(h);
                             Serial.print("%, Conc. of H2 in PPM: ");
                             Serial.print(H2);
                             Serial.print(", Conc. of LPG in PPM: ");
                             Serial.print(LPG);
                             Serial.print(", Conc. of CO in PPM: ");
                             Serial.print(CO);
                             Serial.print(", Conc. of NH3 in PPM ");
                             Serial.print(NH3);
                             Serial.print(", Conc. of TOULENE in PPM ");
                             Serial.print(Toulene);
                             Serial.print(", Conc. of CO2 in PPM ");
                             Serial.print(CO2 + 400);
                             Serial.println(" Send to Thingspeak.");
                        } 
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(20000);
}

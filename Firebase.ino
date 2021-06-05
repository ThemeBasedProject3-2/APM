#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include<DHT.h>
#include<MQUnifiedsensor.h>

#define DHTPIN 2                   
#define FIREBASE_HOST "led123-af4d8-default-rtdb.firebaseio.com" //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "ilyWvNAvn3XiD3FdYGwXaIONMJKuCLaqtLrZOwX3"      //Your Firebase Database Secret goes here           

#define WIFI_SSID "Vprasad"                                               //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "8885995407"   

//Password of your wifi network 

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


 DHT dht(DHTPIN, DHT11);

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

MQUnifiedsensor MQ6(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type1);
MQUnifiedsensor MQ7(board, Voltage_Resolution, ADC_Bit_Resolution,pin,type2);
MQUnifiedsensor MQ135(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type3);

int val = 0;

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



void setup() {

  Serial.begin(115200);      
  // Select the same baud rate if you want to see the datas on Serial Monitor
  
        initMQ6();
        initMQ7();
        initMQ135();
        dht.begin();
        
       pinMode(D1, OUTPUT);   
       pinMode(D2, OUTPUT);
       pinMode(D3, OUTPUT);  
       
       MakeLow();
      
  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(1000);
}

void loop() { 

// Firebase Error Handling And Writing Data At Specifed Path**
  MakeLow();
  float t = dht.readTemperature();
  float h =  dht.readHumidity();
  
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
   
  

   if ((isnan(t))||(isnan(h))) 
    {
          Serial.println("Failed to read from DHT sensor!");
          delay(2500);
          return;
     }    
  
 
 
if (Firebase.setFloat(firebaseData, "/sample", val)) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("val = ");
               Serial.println(val);
               Serial.println("\n");
               
            
               delay(1000);

     }
 
else {        
    Serial.println(firebaseData.errorReason());
  }


if (Firebase.setString(firebaseData, "/temp", String(val) + "," + String(t))) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("t = ");
               Serial.println(t);
               Serial.println("\n");
               
            
               delay(1000);

     }

else {        
    Serial.println(firebaseData.errorReason());
  }



if (Firebase.setFloat(firebaseData, "/humidity", String(val) + "," + String(h))) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("h = ");
               Serial.println(h);
               Serial.println("\n");
               
            
               delay(1000);

     }

  else {        
    Serial.println(firebaseData.errorReason());
  }
  
  if (Firebase.setFloat(firebaseData, "/H2", String(val) + "," + String(H2))) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("H2 = ");
               Serial.println(H2);
               Serial.println("\n");
               
            
               delay(1000);

     }

else {        
    Serial.println(firebaseData.errorReason());
  }
  
  if (Firebase.setFloat(firebaseData, "/LPG", String(val) + "," + String(LPG))) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("t = ");
               Serial.println(t);
               Serial.println("\n");
               
            
               delay(1000);

     }

else {        
    Serial.println(firebaseData.errorReason());
  }
  
  if (Firebase.setFloat(firebaseData, "/CO", CO)) {    // On successful Write operation, function returns 1  
               Serial.println("Value Uploaded Successfully");
               Serial.print("CO = ");
               Serial.println(CO);
               Serial.println("\n");
               
            
               delay(1000);

     }

else {        
    Serial.println(firebaseData.errorReason());
  }

 val += 1;
 
}

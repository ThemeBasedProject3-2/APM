#include<ESP8266Wifi.h>

String apiKey = ""; //Enter the Api key here

String ssid = ""; //Enter your network name

String pass = ""; //Enter your network password

const char* server = "api.thingspeak.com";

WiFiClient client;

void setup()
{
   Serial.begin(115200); // sets the baud rate for the serial communication btw micro-controller and pc.
   delay(10);
	
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
	//Parameter 1 and 2 should be defined here.

	if (client.connect(server,80)) // This function is used to connect to the server.
	{
		 String postStr = apiKey;
                 postStr +="&field1=";
                 postStr += String(paramater 1); // paramater 1 and 2 are the parameters we want to plot.
                 postStr +="&field2=";
                 postStr += String(parameter 2); // parameter 1 and 2 may vary according to the the sensor used.
                 postStr += "\r\n\r\n";

		//Writing the data read from the sensor to the server.

		 client.print("POST /update HTTP/1.1\n");
                 client.print("Host: api.thingspeak.com\n");
                 client.print("Connection: close\n");
                 client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                 client.print("Content-Type: application/x-www-form-urlencoded\n");
                 client.print("Content-Length: ");
                 client.print(postStr.length());
                 client.print("\n\n");
                 client.print(postStr);
		
			
	}
	client.stop();
	delay(20000);
}

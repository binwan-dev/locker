#include <ESP8266WiFi.h>

char ssid[]="Bin";
char password[]="wanbin1994";
IPAddress staticIP(192,168,3,200);
IPAddress gateway(192,168,3,1);
IPAddress subnet(255,255,255,0);
IPAddress server(192,168,3,105);
int port=3233;
WiFiClient client;

void setup()
{
	Serial.begin(9600);

	Serial.printf("Connecting to %s\n", ssid);
  WiFi.mode(WIFI_STA);
	WiFi.config(staticIP,gateway,subnet);
	//WiFi.setAutoReconnect(true);
	WiFi.begin(ssid,password);
	while(WiFi.status()!=WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println();
	Serial.print("Connected, IP address: ");
	Serial.println(WiFi.localIP());
}

void loop()
{
	if(!client.connected())
	{
		connectServer();
	}

  if(client.available())
  {
  	String line = client.readStringUntil('\n');
    Serial.print(line);
  }
}

void connectServer()
{
	while(!client.connected())
	{
		client.connect(server,port);
		Serial.printf("Try connect server(%s:%d)...\n",server,port);
		delay(500);
	}
}

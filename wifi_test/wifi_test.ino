#include <#WiFi.h>

char ssid[]="WA";
char password[]="wivia1868";
IPAddress staticIP(192.168.3.200);
IPAddress gateway(192.168.3.1);
IPAddress subnet(255.255.255.0);
IPAddress server(192.168.3.173);
int port=3233;
WiFiClient client;

void setup()
{
	Serial.begin(9600);

	Serial.printf("Connecting to %s\n", ssid);
	WiFi.config(staticIP,gateway,subnet);
	WiFi.setAutoReconnect(true);
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
	if(!client.available())
	{
		connectServer();
	}

	char c=client.read();
	Serial.print(c);
}

void connectServer()
{
	while(!client.available())
	{
		client.connect(server,port);
		Serial.printf("Try connect server(%s:%d)...\n",server,port);
		delay(500);
	}
}

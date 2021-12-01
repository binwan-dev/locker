#include "Stepper.h"
#include "ClientSocket.h"

Stepper *stepper = NULL;
ClientSocket *socket = NULL;

void setup()
{
	Serial.begin(9600);

	socket = newClientSocket();
	socket->ConnectWiFi();

	stepper = new Stepper(D0, D1, D2, D3, 3);
}

void loop()
{
	String signal = socket->ReadContent('\n');
	if (signal == "")
	{
		return;
	}

	if (signal == "open")
	{
		openDoor();
	}
}

ClientSocket *newClientSocket()
{
	SocketConfig config;
	config.SSID = "WA";
	config.Password = "wivia1868";
	config.StaticIP = IPAddress(192, 168, 3, 200);
	config.Dns = IPAddress(255, 255, 255, 0);
	config.Gateway = IPAddress(192, 168, 3, 1);
	config.Server = IPAddress(192, 168, 3, 93);
	return new ClientSocket(config);
}

void openDoor()
{
	Serial.println("Door Opening...");
	stepper->Forward(1100);
	stepper->Back(1100);
	stepper->Reset();
	Serial.println("Door Opened!");
}
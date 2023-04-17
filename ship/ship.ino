#include "Stepper.h"
#include "BServo.h"
#include "ClientSocket.h"

Stepper *stepper = NULL;
BServo *bservo = NULL;
ClientSocket *socket = NULL;

void setup()
{
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);

	socket = newClientSocket();
	socket->ConnectWiFi();

	// stepper = new Stepper(D5, D6, D7, D8, 3);
	bservo = new BServo(2);
}

// void loop()
// {
// String signal = socket->ReadContent('\n');
// if (signal == "")
// {
// 	return;
// }

// if (signal == "opendoor")
// {
// 	digitalWrite(LED_BUILTIN, 0);
// 	openDoor();
// 	digitalWrite(LED_BUILTIN, 1);
// }
// }

void loop()
{
	String content = socket->ReadContent('\n');
	if (content == "")
	{
		delay(500);
		return;
	}
}

ClientSocket *newClientSocket()
{
	SocketConfig config;
	config.SSID = "Bin";
	config.Password = "wanbin1994";
	config.StaticIP = IPAddress(192, 168, 3, 200);
	config.Dns = IPAddress(255, 255, 255, 0);
	config.Gateway = IPAddress(192, 168, 3, 1);
	config.Server = IPAddress(8, 210, 153, 169);
	config.Port = 5663;
	return new ClientSocket(config);
}

void openDoor()
{
	Serial.println("Door Opening...");
	bservo->Forward(4);
	bservo->Reset();
	bservo->Back(4);
	bservo->Reset();
	Serial.println("Door Opened!");
}
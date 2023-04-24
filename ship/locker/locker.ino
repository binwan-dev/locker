#include "ClientSocket.h";
#include "Model.h";
#include "BServo.h";

BServo *bservo = NULL;
ClientSocket *socket = NULL;


static const uint8_t GPIO_DOOR = 5;
#define GPIO_DOOR GPIO_DOOR

bool Registed = false;

void setup()
{
  	pinMode(LED_BUILTIN, OUTPUT);
  	pinMode(GPIO_DOOR, OUTPUT);

  	Serial.begin(9600);
  	Serial.println("starting....");

  	socket = newClientSocket();
  	socket->ConnectWiFi();

    bservo = new BServo(GPIO_DOOR);
}

ClientSocket *newClientSocket()
{
  SocketConfig config;
  config.SSID = "Bin";
  config.Password = "wanbin1994";
  config.StaticIP = IPAddress(192, 168, 3, 200);
  config.Dns = IPAddress(255, 255, 255, 0);
  config.Gateway = IPAddress(192, 168, 3, 1);
  config.Server = IPAddress(192, 168, 3, 252);
  config.Port = 5663;
  IOTInfo iotInfo;
  iotInfo.IOTType = IOTType_Door;
  return new ClientSocket(config, iotInfo);
}

void loop()
{
  delay(1000);
  if (!socket->ServerConnected())
  {
    socket->ConnectServer();
    Registed=false;
  }
  if (socket->ServerConnected() && !Registed)
  {
    socket->RegisterDevice();
    Registed = true;
  }

  IOTServerCommand command;
  if(!socket->ReadCommand('\n', &command))
  {
    return;
  }
  switch(command.Command)
  {
    case IOTCommand_OpenDoor:
      openDoor();
      break;
    default:
      Serial.println("invalid command from server!");
      break;
  }
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
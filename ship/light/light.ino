#include <BinHomeLib.h>

static const uint8_t GPIO_LIGHT = 5;
#define GPIO_LIGHT GPIO_LIGHT

bool Registed = false;
ClientSocket *socket = NULL;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_LIGHT, OUTPUT);
  digitalWrite(GPIO_LIGHT, HIGH);

  Serial.begin(9600);
  Serial.println("starting....");

  socket = newClientSocket();  
  socket->ConnectWiFi();
}

ClientSocket *newClientSocket()
{
  SocketConfig config;
  config.SSID = "Bin";
  config.Password = "wanbin1994";
  config.Server = IPAddress(192, 168, 3, 252);
  config.Port = 5663;
  IOTInfo iotInfo;
  iotInfo.IOTType = IOTType_Light;
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
    case IOTCommand_LightOn:
      lightOn();
      break;
    case IOTCommand_LightOff:
      lightOff();
      break;
    default:
      Serial.println("invalid command from server!");
      break;
  }
}

void lightOn()
{
  Serial.println("light on");
  digitalWrite(GPIO_LIGHT, HIGH);
}

void lightOff()
{
  Serial.println("light off");
  digitalWrite(GPIO_LIGHT, LOW);
}
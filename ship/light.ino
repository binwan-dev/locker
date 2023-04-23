#include "ClientSocket.h";
#include "Model.h";
#include <ArduinoJson.h>

static const uint8_t GPIO_LIGHT = 5;
#define GPIO_LIGHT GPIO_LIGHT

bool Registed = false;
bool GPIO_LIGHT_INIT=false;
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
  config.StaticIP = IPAddress(192, 168, 3, 200);
  config.Dns = IPAddress(255, 255, 255, 0);
  config.Gateway = IPAddress(192, 168, 3, 1);
  config.Server = IPAddress(192, 168, 3, 252);
  config.Port = 5663;
  return new ClientSocket(config);
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
    socket->RegisterDevice(IOTType_Light);
    Registed = true;
  }

  String payload = socket->ReadContent('\n');
  if (payload == "")
  {
    return;
  }

  IOTServerCommand command=parse(payload);  
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

IOTServerCommand parse(String payloadStr)
{
  Serial.println(payloadStr);
  IOTServerCommand command;
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, payloadStr);
  if(error)
  {
    return command;
  }
  
  command.Command=doc["Command"];
  //command.MacAddress=doc["MacAddress"];
  return command;
}

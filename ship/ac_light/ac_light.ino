#include <BinHomeLib.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <MideaIR.h>
#include <IRsend.h>

#define GPIO_AC       14

static const uint8_t GPIO_LIGHT = 5;
#define GPIO_LIGHT GPIO_LIGHT

bool Registed = false;
ClientSocket *socket = NULL;

IRsend irsend(GPIO_AC);
MideaIR remote_control(&irsend);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GPIO_LIGHT, OUTPUT);
  pinMode(GPIO_AC, OUTPUT);
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
  iotInfo.IOTType = IOTType_Ac_Light;
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
    case IOTCommand_AcOpen:
      acOpen();
      break;
    case IOTCommand_AcSetTemperature:
      acSetTemperature(command.Body);
      break;
    case IOTCommand_AcClose:
      acClose();
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

void acOpen()
{
  Serial.println("ac open");
  remote_control.turnON();
  delay(2000);
  remote_control.seTurboMode();
}

void acSetTemperature(String bodyStr)
{
  IOTAcSetTemperatureDto dto = parse(bodyStr);
  Serial.println("ac set temperature to " + dto.Temperature);
  remote_control.setTemperature(dto.Temperature);
  remote_control.emit();
  delay(1000);
  remote_control.setMode(mode_cool);
}

void acClose()
{
  Serial.println("ac close");
  remote_control.turnOFF();
}

IOTAcSetTemperatureDto parse(String contentStr)
{
  Serial.println(contentStr);
  IOTAcSetTemperatureDto dto;
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, contentStr);
  if(error)
  {
    return dto;
  }
  
  dto.Temperature=doc["Temperature"];
  return dto;
}

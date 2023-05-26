#include <BinHomeLib.h>
#include <ArduinoJson.h>

static const uint8_t GPIO_LIGHT = 5;
#define GPIO_LIGHT GPIO_LIGHT

static const uint8_t GPIO_AC = 4;
#define GPIO_AC GPIO_AC

bool Registed = false;
ClientSocket *socket = NULL;
IRsendMeidi *irsendmeidi = NULL;

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

  irsendmeidi = new IRsendMeidi(GPIO_AC);
  irsendmeidi->begin_2();   //初始化
  irsendmeidi->setZBPL(40); //设置红外载波频率，单位kHz,不调用此函数则默认38，由于未知原因，我设置为40，示波器测得频率为38左右，当发送信号后没反应时，尝试更改此值。
  irsendmeidi->setCodeTime(500,1600,550,4400,4400,5220); //设置信号的高低电平占比，分别为标记位，1位，0位，前导码低电平，前导码高电平，间隔码高电平
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
  irsendmeidi->setPowers(1);
}

void acSetTemperature(String bodyStr)
{
  IOTAcSetTemperatureDto dto = parse(bodyStr);
  Serial.println("ac set temperature to " + dto.Temperature);
  irsendmeidi->setTemps(dto.Temperature);
}

void acClose()
{
  Serial.println("ac close");
  irsendmeidi->setPowers(0);
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

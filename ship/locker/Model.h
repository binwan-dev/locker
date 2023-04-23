#include <Arduino.h>

struct DeviceInfo
{
    char *MacAddress;
};

enum IOTCommand
{
    Register = 1
};

static const int IOTType_Light = 2; // light
static const int IOTType_Door = 1;
#define IOTType_Light IOTType_Light
#define IOTType_Door IOTType_Door

#define IOTCommand_Register 1
#define IOTCommand_Heartbeat 999
#define IOTCommand_OpenDoor 2
#define IOTCommand_LightOn 3
#define IOTCommand_LightOff 4

struct IOTServerCommand
{
    int Command;
    char* MacAddress;
    char* Body;
};
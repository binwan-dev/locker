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
#define IOTType_Light IOTType_Light

#define IOTCommand_Register 1
#define IOTCommand_LightOn 3
#define IOTCommand_LightOff 4

struct IOTServerCommand
{
    int Command;
    char* MacAddress;
    char* Body;
};
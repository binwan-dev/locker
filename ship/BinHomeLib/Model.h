#include <Arduino.h>

#ifndef MODEL_H
#define MODEL_H
struct DeviceInfo
{
    char *MacAddress;
};

enum IOTCommand
{
    Register = 1
};

static const int IOTType_Door = 1;
static const int IOTType_Light = 2; // light
static const int IOTType_Ac = 3;
static const int IOTType_Ac_Light = 4;
#define IOTType_Light IOTType_Light
#define IOTType_Door IOTType_Door
#define IOTType_Ac IOTType_Ac
#define IOTType_Ac_Light IOTType_Ac_Light

#define IOTCommand_Register 1
#define IOTCommand_Heartbeat 999
#define IOTCommand_OpenDoor 2
#define IOTCommand_LightOn 3
#define IOTCommand_LightOff 4
#define IOTCommand_AcOpen 5
#define IOTCommand_AcSetTemperature 6
#define IOTCommand_AcClose 7

struct IOTServerCommand
{
    int Command;
    const char* MacAddress;
    const char* Body;
};

struct IOTAcSetTemperatureDto
{
    int Temperature;
};
#endif

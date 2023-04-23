#ifndef _ClientSocket_H__
#define _ClientSocket_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>

struct SocketConfig
{
    char *SSID;
    char *Password;
    IPAddress StaticIP;
    IPAddress Dns;
    IPAddress Gateway;
    IPAddress Server;
    int Port;
};

struct IOTInfo
{
    int IOTType;
}

class ClientSocket
{
private:
    SocketConfig _config;
    IOTInfo _iotInfo;
    WiFiClient _client;
    int _loopCount;
    bool _recvHeartbeat;
    void sendHeartbeat();

public:
    bool ServerConnected();
    ClientSocket(SocketConfig config, IOTInfo iotInfo);
    void ConnectWiFi();
    void ConnectServer();
    IOTServerCommand *ReadCommand(char split);
    void RegisterDevice();
};

#endif
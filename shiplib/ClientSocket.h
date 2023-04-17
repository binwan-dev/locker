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

class ClientSocket
{
private:
    SocketConfig _config;
    WiFiClient _client;
    int _loopCount;
    bool _recvHeartbeat;
    void sendHeartBeat();

public:
    bool ServerConnected();
    ClientSocket(SocketConfig config);
    void ConnectWiFi();
    void ConnectServer();
    String ReadContent(char split);
    void RegisterDevice();
};

#endif
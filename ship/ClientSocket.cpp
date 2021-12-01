#include <ESP8266WiFi.h>
#include "ClientSocket.h"

ClientSocket::ClientSocket(SocketConfig config)
{
    this->_config = config;
}

void ClientSocket::ConnectWiFi()
{
    Serial.printf("Connecting to %s\n", this->_config.SSID);
    WiFi.mode(WIFI_STA);
    WiFi.config(this->_config.StaticIP, this->_config.Gateway, this->_config.Dns);
    //WiFi.setAutoReconnect(true);
    WiFi.begin(this->_config.SSID, this->_config.Password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void ClientSocket::connectServer()
{
    while (!_client.connected())
    {
        _client.connect(this->_config.Server, this->_config.Port);
        Serial.printf("Try connect server(%s:%d)...\n", this->_config.Server.toString(), this->_config.Port);
        delay(500);
    }
}

String ClientSocket::ReadContent(char split)
{
    if (!_client.connected())
    {
        this->connectServer();
    }

    if (_client.available() < 1)
    {
        return "";
    }

    return this->_client.readStringUntil(split);
}
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "ClientSocket.h"
#include "Model.h"

ClientSocket::ClientSocket(SocketConfig config)
{
    this->_config = config;
}

void ClientSocket::ConnectWiFi()
{
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.printf("Connecting to %s\n", this->_config.SSID);
    WiFi.mode(WIFI_STA);
    WiFi.config(this->_config.StaticIP, this->_config.Gateway, this->_config.Dns);
    WiFi.setAutoReconnect(true);
    WiFi.begin(this->_config.SSID, this->_config.Password);
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(LED_BUILTIN, HIGH);
}

bool ClientSocket::ServerConnected()
{
    return _client.connected();
}

void ClientSocket::ConnectServer()
{
    while (!_client.connected())
    {
        _client.connect(this->_config.Server, this->_config.Port);
        Serial.printf("Try connect server(%s:%d)...\n", this->_config.Server.toString(), this->_config.Port);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.printf("server connected! address: %s\n", _client.localIP().toString());
}

void ClientSocket::RegisterDevice()
{
    String empty = "";
    String message = empty + "{\"Command\":" + IOTCommand_Register + ", \"Type\": " + IOTType_Light + ",\"MacAddress\":\"" + String(WiFi.macAddress()) + "\"}";
    Serial.printf("send register data");
    _client.println(message);
}

String ClientSocket::ReadContent(char split)
{
    if (_client.available() < 1)
    {
        delay(500);
        _loopCount++;
        if (_loopCount == 120)
        {
            if (_recvHeartbeat)
            {
                digitalWrite(LED_BUILTIN, LOW);
                _client.println("ping");
                _recvHeartbeat = false;
            }
            else
            {
                delay(1000);
                digitalWrite(LED_BUILTIN, HIGH);
            }
            // Serial.println("Send heart beat...");
            _loopCount = 0;
        }
        return "";
    }

    return this->_client.readStringUntil(split);
}

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include "ClientSocket.h"
#include <ArduinoJson.h>

ClientSocket::ClientSocket(SocketConfig config, IOTInfo iotInfo)
{
    this->_config = config;
    this->_iotInfo = iotInfo;
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
    String message = empty + "{\"Command\":" + IOTCommand_Register + ", \"Type\": " + _iotInfo.IOTType + ",\"MacAddress\":\"" + String(WiFi.macAddress()) + "\"}";
    Serial.printf("send register data\n");
    _client.println(message);
}

void ClientSocket::sendHeartbeat()
{
    String empty = "";
    String message = empty + "{\"Command\":" + IOTCommand_Heartbeat + ", \"Type\": " + _iotInfo.IOTType + ",\"MacAddress\":\"" + String(WiFi.macAddress()) + "\"}";   
    Serial.printf("send heartbeat\n");
    _client.println(message);
}

bool ClientSocket::ReadCommand(char split, IOTServerCommand *command)
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
                sendHeartbeat();
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
        return false;
    }

    String payload = this->_client.readStringUntil(split);
    if(payload=="")
    {
        return false;
    }

    *command = parse(payload);
    if(command->Command==IOTCommand_Heartbeat)
    {
        Serial.println(command->Body);
        return false;
    }

    return true;
}

IOTServerCommand ClientSocket::parse(String payloadStr)
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
  command.MacAddress=doc["MacAddress"];
  command.Body=doc["Body"];
  return command;
}

using System.Net.Sockets;
using System.Text;
using System.Text.Json;
using BinHome.Server.Infrastructure.Socketing;

namespace BinHome.Server.Infrastructure;

public class IOTManager : ITcpEventHandler
{
    private readonly IDictionary<string, TcpConnection> _iotTcpConnections;
    private readonly IDictionary<string, IOTMessage> _iotDevices;

    public IOTManager()
    {
        _iotTcpConnections = new Dictionary<string, TcpConnection>();
        _iotDevices = new Dictionary<string, IOTMessage>();
    }

    public IEnumerable<IOTMessage> Devices => _iotDevices.Values;

    public void HandleAccepted(TcpConnection connection)
    {
        if (connection == null || !connection.Connected)
            return;

        if (_iotTcpConnections.ContainsKey(connection.ID))
            _iotTcpConnections[connection.ID] = connection;
        else
            _iotTcpConnections.Add(connection.ID, connection);
    }

    public void HandleClosed(TcpConnection connection, SocketError socketError, Exception? ex)
    {
        if (_iotTcpConnections.ContainsKey(connection.ID))
            _iotTcpConnections.Remove(connection.ID);

        var iotDevice = _iotDevices.Values.FirstOrDefault(p => p.Connection.ID == connection.ID);
	if(iotDevice!=null)
            _iotDevices.Remove(iotDevice.MacAddress);
    }

    public void HandleReceived(TcpConnection connection, ArraySegment<byte> data)
    {
        var iotMessage = parse(data);
	if(iotMessage==null)
            return;
        iotMessage.Connection = connection;

        switch (iotMessage.Command)
        {
	    case IOTCommand.Regist:
                HandleRegister(iotMessage);
                break;
        }   
    }

    public void OpenDoor(string deviceMac, string content = "")
	=> sendSimpleCommandToDevice(deviceMac, content, IOTCommand.OpenDoor, IOTType.Door);

    public void LightOn(string deviceMac, string content = "")
	=> sendSimpleCommandToDevice(deviceMac, content, IOTCommand.LightOn, IOTType.Light);
    
    public void LightOff(string deviceMac, string content = "")
	=> sendSimpleCommandToDevice(deviceMac, content, IOTCommand.LightOff, IOTType.Light);

    private void sendSimpleCommandToDevice(string deviceMac,string content,IOTCommand command,IOTType iotType)
    {
	var device = checkAndGetDevice(deviceMac, command, iotType);
        var reply = new IOTReply()
        {
            Command = command,
            MacAddress = deviceMac,
            Body = content
        };
        device.Connection.QueueMessage(Encoding.UTF8.GetBytes($"{JsonSerializer.Serialize(reply)}\n"));
    }

    private IOTMessage checkAndGetDevice(string deviceMac, IOTCommand command, IOTType iotType)
    {
	if(!_iotDevices.TryGetValue(deviceMac,out IOTMessage? device)||device==null)
            throw new InvalidOperationException("Cannot found device!");
	if(device.Type!=iotType)
            throw new InvalidOperationException("Device type error!");
	if(device.Connection==null||!device.Connection.Connected)
            throw new InvalidOperationException("Tcp connection not connected!");
        return device;
    }

    private void HandleRegister(IOTMessage message)
    {
	Console.WriteLine(message.MacAddress);
	if(_iotDevices.ContainsKey(message.MacAddress))
            _iotDevices[message.MacAddress] = message;
	else
            _iotDevices.Add(message.MacAddress, message);
    }
    
    private IOTMessage? parse(ArraySegment<byte> data)
    {
        var content = Encoding.UTF8.GetString(data);
	if(String.IsNullOrWhiteSpace(content))
            return null;
	
        var iotMessage = System.Text.Json.JsonSerializer.Deserialize<IOTMessage>(content);
        if(iotMessage==null)
            return null;
        return iotMessage;
    }
}

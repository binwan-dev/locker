using BinHome.Server.Infrastructure.Socketing;

namespace BinHome.Server.Infrastructure;

public class IOTMessage
{
    public IOTCommand Command{ get;set; }

    public string MacAddress { get; set; } = string.Empty;

    public IOTType Type{ get;set; }

    public TcpConnection Connection { get; set; } = null!;

    public string Body { get; set; } = string.Empty;
}

public class IOTReply
{
    public IOTCommand Command{ get;set; }

    public string MacAddress { get; set; } = string.Empty;

    public string Body { get; set; } = string.Empty;
}

public enum IOTCommand
{
    Regist=1,
    OpenDoor=2,
    LightOn=3,
    LightOff=4,
}

public enum IOTType
{
    Door=1,
    Light=2
}

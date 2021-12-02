using BinHome.Server.Infrastructure;
using BinHome.Server.Infrastructure.Socketing;

namespace BinHome.Server.Handlers
{
    public class LockerHandler
    {
        private readonly TcpEventHandler _tcpEventHandler;

        public LockerHandler(ITcpEventHandler tcpEventHandler)
        {
            _tcpEventHandler = (TcpEventHandler)tcpEventHandler;
        }

        public void OpenDoor()
        {
            var command = "opendoor\n";
            _tcpEventHandler.SendMessage(System.Text.Encoding.UTF8.GetBytes(command));
        }
    }
}
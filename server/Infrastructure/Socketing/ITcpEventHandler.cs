using System.Net.Sockets;

namespace BinHome.Server.Infrastructure.Socketing
{
    public interface ITcpEventHandler
    {
        void HandleAccepted(TcpConnection connection);

        void HandleReceived(TcpConnection connection, ArraySegment<byte> data);

        void HandleClosed(TcpConnection connection, SocketError socketError, Exception? ex);

    }
}
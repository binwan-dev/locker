using System.Net.Sockets;
using BinHome.Server.Infrastructure.Socketing;

namespace BinHome.Server.Infrastructure
{
    public class TcpEventHandler : ITcpEventHandler
    {
        private TcpConnection? _connection;

        public void SendMessage(byte[] message)
        {
            if (_connection == null)
                throw new ArgumentException("Connection has broken!");
            _connection.QueueMessage(message);
        }

        public void HandleAccepted(TcpConnection connection)
        {
            _connection = connection;
        }

        public void HandleClosed(TcpConnection connection, SocketError socketError, Exception? ex)
        {
            _connection = null;
        }

        public void HandleReceived(TcpConnection connection, ArraySegment<byte> data)
        {
        }
    }
}
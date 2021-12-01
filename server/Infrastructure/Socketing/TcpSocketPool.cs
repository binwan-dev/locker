using System.Net.Sockets;

namespace BinHome.Server.Infrastructure.Socketing
{
    public class TcpSocketPool
    {
        private readonly IDictionary<string, Socket> _socketDict;

        public TcpSocketPool()
        {
            _socketDict = new Dictionary<string, Socket>();
        }

        public Socket? GetSocket(string endPoint)
        {
            if (string.IsNullOrWhiteSpace(endPoint))
                return null;
            if (!_socketDict.TryGetValue(endPoint, out Socket? socket))
                return null;

            return socket;
        }

        public Socket? GetFirst()
        {
            if (_socketDict.Count == 0)
                return null;
            return _socketDict.Values.First();
        }

        public void AddSocket(Socket socket)
        {

        }
    }
}
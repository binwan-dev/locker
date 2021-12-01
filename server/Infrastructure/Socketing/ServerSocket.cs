using System.Net;
using System.Net.Sockets;

namespace BinHome.Server.Infrastructure.Socketing
{
    public class ServerSocket
    {
        private readonly SocketOptions _options;
        private readonly SocketAsyncEventArgs _acceptArgs;
        private readonly Socket _socket;
        private readonly ILogger<ServerSocket> _log;
        private readonly TcpSocketPool _socketPool;
        private static int _accepting = 0;

        public ServerSocket(SocketOptions options, ILogger<ServerSocket> log, TcpSocketPool socketPool)
        {
            _options = options;
            _log = log;
            _socketPool = socketPool;
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            _acceptArgs = new SocketAsyncEventArgs();
            _acceptArgs.Completed += onAccepted;
        }

        public void Start()
        {
            _socket.Bind(new IPEndPoint(IPAddress.Parse(_options.Host ?? "0.0.0.0"), _options.Port));
            _socket.Listen();

            tryAccept();
        }

        private void tryAccept()
        {
            try
            {
                if (Interlocked.CompareExchange(ref _accepting, 1, 0) != 0)
                    return;

                if (!_socket.AcceptAsync(_acceptArgs))
                    Task.Factory.StartNew(() => onAccepted(_socket, _acceptArgs));
            }
            catch (Exception ex)
            {
                _log.LogError(ex, $"Try accept has an unknow error!");
                Interlocked.Exchange(ref _accepting, 0);
            }
        }

        private void onAccepted(object? sender, SocketAsyncEventArgs socket)
        {
            if (socket.SocketError != SocketError.Success)
            {
                _log.LogError($"Server accept failed! SocketError: {socket.SocketError}");
                tryAccept();
                return;
            }


        }

    }
}
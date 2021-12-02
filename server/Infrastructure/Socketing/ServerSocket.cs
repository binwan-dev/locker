using System.Net;
using System.Net.Sockets;
using Microsoft.Extensions.Options;

namespace BinHome.Server.Infrastructure.Socketing
{
    public class ServerSocket
    {
        private readonly SocketOptions _options;
        private readonly SocketAsyncEventArgs _acceptArgs;
        private readonly Socket _socket;
        private readonly ILogger<ServerSocket> _log;
        private readonly ILogger<TcpConnection> _tcpLog;
        private readonly ITcpEventHandler? _tcpEventHandler;

        public ServerSocket(IOptions<SocketOptions> options, ILogger<ServerSocket> log, ILogger<TcpConnection> tcpLog, ITcpEventHandler? tcpEventHandler)
        {
            _options = options.Value;
            _tcpEventHandler = tcpEventHandler;
            _log = log;
            _tcpLog = tcpLog;
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            _acceptArgs = new SocketAsyncEventArgs();
            _acceptArgs.Completed += onAccepted;
        }

        public void Start()
        {
            try
            {
                _socket.Bind(new IPEndPoint(IPAddress.Parse(_options.Host ?? "0.0.0.0"), _options.Port));
                _socket.Listen();
                _log.LogInformation($"Socket listen point -> {_options.Host}-{_options.Port}");
            }
            catch (Exception ex)
            {
                _log.LogError(ex, "Listen endpoint failed!");
            }

            startAccepting();
        }

        private void startAccepting()
        {
            try
            {
                if (!_socket.AcceptAsync(_acceptArgs))
                    onAccepted(_socket, _acceptArgs);
            }
            catch (Exception ex)
            {
                _log.LogError(ex, $"Accept has an unknow error!");
                startAccepting();
            }
        }

        private void onAccepted(object? sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError != SocketError.Success)
            {
                _log.LogError($"Server accept failed! SocketError: {args.SocketError}");
                args.AcceptSocket?.Shutdown(SocketShutdown.Both);
                args.AcceptSocket?.Dispose();
                return;
            }

            try
            {
                var socket = args.AcceptSocket;
                Task.Factory.StartNew(() =>
                {
                    try
                    {
                        var tcpConnection = new TcpConnection(socket, _options, _tcpLog, _tcpEventHandler);
                        _tcpEventHandler?.HandleAccepted(tcpConnection);
                    }
                    catch (Exception)
                    {
                        socket?.Shutdown(SocketShutdown.Both);
                        socket?.Close();
                        socket?.Dispose();
                    }
                });
                args.AcceptSocket = null;
            }
            catch (Exception ex)
            {
                _log.LogError(ex, "Process accepted has an unknow error!");
            }
            finally
            {
                startAccepting();
            }
        }

    }
}
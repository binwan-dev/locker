using System.Collections.Concurrent;
using System.Net.Sockets;

namespace BinHome.Server.Infrastructure.Socketing
{
    public class TcpConnection
    {
        private readonly Socket _socket;
        private readonly SocketAsyncEventArgs _sendArgs;
        private readonly SocketAsyncEventArgs _receiveArgs;
        private readonly ILogger<TcpConnection> _log;
        private readonly SocketOptions _options;
        private readonly ITcpEventHandler? _tcpEventHandler;
        private readonly ConcurrentQueue<byte[]> _sendQueue;
        private readonly MemoryStream _sendStream;
        private int _sending = 0;
        private int _receiving = 0;
        private int _closing = 0;

        public TcpConnection(Socket? socket, SocketOptions options, ILogger<TcpConnection> log, ITcpEventHandler? tcpEventHandler)
        {
            _socket = socket ?? throw new ArgumentNullException("Tcp socket cannot be null!");
            _log = log;
            _options = options;
            _tcpEventHandler = tcpEventHandler;
            _sendQueue = new ConcurrentQueue<byte[]>();
            _sendStream = new MemoryStream();
            _sendArgs = new SocketAsyncEventArgs();
            _sendArgs.Completed += OnSendCompleted;
            _receiveArgs = new SocketAsyncEventArgs();
            _receiveArgs.Completed += OnReceiveCompleted;
            _sendArgs.SetBuffer(new byte[_options.ReceiveBufferSize]);

            tryReceive();
        }

        public void QueueMessage(byte[] msg)
        {
            if (msg.Length == 0)
                return;
            _sendQueue.Enqueue(msg);

            trySend();
        }

        private void trySend()
        {

            try
            {
                if (Interlocked.CompareExchange(ref _sending, 1, 0) != 0)
                    return;

                send();
            }
            catch (Exception ex)
            {
                close(SocketError.ConnectionRefused, "Try sending data has an unknow error!", ex);
                return;
            }
            finally
            {
                Interlocked.Exchange(ref _sending, 0);
            }
        }
        private void send()
        {
            _sendStream.SetLength(0);
            _sendStream.Seek(0, SeekOrigin.Begin);
            while (true)
            {
                if (_sendStream.Length > _options.SendBufferSize)
                    break;
                if (!_sendQueue.TryDequeue(out byte[]? msg))
                    break;

                _sendStream.Write(msg, 0, msg.Length);
            }

            if (_sendStream.Length == 0)
                return;

            _sendArgs.SetBuffer(_sendStream.ToArray());
            if (!_socket.SendAsync(_sendArgs))
                OnSendCompleted(_socket, _sendArgs);
        }

        private void OnSendCompleted(object? sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError != SocketError.Success)
            {
                close(args.SocketError, "Send message has failed!", null);
                return;
            }

            send();
        }

        private void tryReceive()
        {
            try
            {
                if (Interlocked.CompareExchange(ref _receiving, 1, 0) != 0)
                    return;

                if (!_socket.ReceiveAsync(_receiveArgs))
                    OnReceiveCompleted(_socket, _receiveArgs);
            }
            catch (Exception ex)
            {
                close(SocketError.ConnectionRefused, "Receive message has an unknow error!", ex);
                return;
            }
            finally
            {
                Interlocked.Exchange(ref _receiving, 0);
            }
        }

        private void OnReceiveCompleted(object? sender, SocketAsyncEventArgs args)
        {
            if (args.SocketError != SocketError.Success)
            {
                close(args.SocketError, "Receive message failed!", null);
                return;
            }

            try
            {
                var buffer = new ArraySegment<byte>(args.Buffer ?? new byte[0], 0, args.BytesTransferred);
                _tcpEventHandler?.HandleReceived(this, buffer);
            }
            catch (Exception ex)
            {
                _log.LogError(ex, "Message received has an unknow error!");
            }
            finally
            {
                tryReceive();
            }
        }

        private void close(SocketError socketError, string message, Exception? ex)
        {
            if (socketError != SocketError.Success)
            {
                _log.LogError(ex, $"{message} SocketError: {socketError}, RemoteEndPoint: {_socket?.RemoteEndPoint?.ToString()}");
            }
            else
            {
                _log.LogInformation(message);
            }

            try
            {
                if (Interlocked.CompareExchange(ref _closing, 1, 0) != 0)
                    return;

                _sendArgs.Completed -= OnSendCompleted;
                _sendArgs.SetBuffer(null);
                _receiveArgs.Completed -= OnReceiveCompleted;
                _receiveArgs.SetBuffer(null);
                _socket?.Close();
                _socket?.Dispose();
                _sendQueue.Clear();
                _sendStream.Close();
                _sendStream.Dispose();
            }
            catch (Exception cex)
            {
                _log.LogError(cex, "Close socket has an unknow error!");
            }
            finally
            {
                _tcpEventHandler?.HandleClosed(this, socketError, ex);
                Interlocked.Exchange(ref _closing, 0);
            }
        }
    }
}
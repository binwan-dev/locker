namespace BinHome.Server.Infrastructure.Socketing
{
    public class SocketOptions
    {
        public SocketOptions()
        {
            SendBufferSize = 1024 * 1024;
            ReceiveBufferSize = 1024 * 1024;
        }

        public string? Host { get; set; }

        public int Port { get; set; }

        public int SendBufferSize { get; set; }

        public int ReceiveBufferSize { get; set; }
    }
}
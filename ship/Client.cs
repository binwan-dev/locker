using System.Net;
using System.Net.Sockets;

var client = new TcpClient();
client.Connect("8.217.197.89", 5663);
var buffer = new byte[1024];
var offest = client.GetStream().Read(buffer, 0, buffer.Length);
Console.WriteLine(System.Text.Encoding.UTF8.GetString(buffer, 0, offest));




Console.WriteLine("hello world");

Console.WriteLine("Shutting down");
Console.WriteLine("Press any key to exit...");
Console.ReadKey();

using BinHome.Server.Infrastructure;
using BinHome.Server.Infrastructure.Socketing;

namespace BinHome.Server
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            builder.Services.AddLogging();
            builder.Services.AddOptions().Configure<SocketOptions>((o) => builder.Configuration.GetSection("SocketOptions").Bind(o));
            var iotManager = new IOTManager();
            builder.Services.AddSingleton<IOTManager>(iotManager);
            builder.Services.AddSingleton<ITcpEventHandler>(iotManager);
            builder.Services.AddSingleton<ServerSocket>();

            builder.Services.AddControllers();
            // Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            var app = builder.Build();

            var serverSocket = app.Services.GetService<ServerSocket>();
            serverSocket?.Start();

            // Configure the HTTP request pipeline.
            // if (app.Environment.IsDevelopment())
            // {
            app.UseSwagger();
            app.UseSwaggerUI();
            // }

            app.UseHttpsRedirection();

            app.UseAuthorization();

            app.MapControllers();

            app.Run();
        }
    }
}

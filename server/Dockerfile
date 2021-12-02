FROM registry.fnlinker.com/fnlinker-public/dotnet-core-aspnet:6.0 AS base
WORKDIR /app
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

FROM registry.fnlinker.com/fnlinker-public/dotnet-core-sdk:6.0 AS build
WORKDIR /src
COPY [".", "server/"]
RUN dotnet restore "server/server.csproj"
COPY . .
WORKDIR "/src/server"
RUN dotnet build "server.csproj" -c Release -o /app

FROM build AS publish
RUN dotnet publish "server.csproj" -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "server.dll"]


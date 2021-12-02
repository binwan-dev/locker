FROM registry.fnlinker.com/fnlinker-public/dotnet-core-aspnet:6.0 AS base
WORKDIR /app
ENV TZ=Asia/Shanghai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

FROM registry.fnlinker.com/fnlinker-public/dotnet-core-sdk:6.0 AS build
WORKDIR /src
COPY [".", "wxserver/"]
RUN dotnet restore "wxserver/wxserver.csproj"
COPY . .
WORKDIR "/src/wxserver"
RUN dotnet build "wxserver.csproj" -c Release -o /app

FROM build AS publish
RUN dotnet publish "wxserver.csproj" -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "wxserver.dll"]


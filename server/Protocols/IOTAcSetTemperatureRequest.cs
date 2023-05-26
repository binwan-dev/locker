namespace BinHome.Server.Protocols;

public record IOTAcSetTemperatureRequest(string MacAddress, int Temperature) : IOTOperRequest(MacAddress);

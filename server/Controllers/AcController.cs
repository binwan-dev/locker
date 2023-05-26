namespace BinHome.Server.Controllers;

[Route("ac")]
public class AcController : ControllerBase
{
    private readonly IOTManager _iotManager;

    public AcController(IOTManager iotManager)
    {
        _iotManager = iotManager;
    }

    [HttpPost("open")]
    public IActionResult Open([FromBody] IOTOperRequest operRequest)
    {
        _iotManager.AcOpen(operRequest.MacAddress);
        return Ok(new { Message = "success" });
    }

    [HttpPost("set/temperature")]
    public IActionResult SetTemperature([FromBody] IOTAcSetTemperatureRequest operRequest)
    {
        _iotManager.AcSetTemperature(operRequest.MacAddress, operRequest.Temperature);
        return Ok(new { Message = "success" });	
    }

    [HttpPost("close")]
    public IActionResult Close(IOTOperRequest operRequest)
    {
        _iotManager.AcClose(operRequest.MacAddress);
        return Ok(new { Message = "success" });
    }
}

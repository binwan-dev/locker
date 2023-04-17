using BinHome.Server.Infrastructure;
using Microsoft.AspNetCore.Mvc;

namespace BinHome.Server.Controllers;

[Route("/light")]
public class LightController : ControllerBase
{
    private readonly IOTManager _iotManager;

    public LightController(IOTManager iotManager)
    {
        _iotManager = iotManager;
    }

    [Route("on/{mac}")]
    public IActionResult On(string mac)
    {
        _iotManager.LightOn(mac);
        return Ok(new { Msg = "success" });
    }

    [Route("off/{mac}")]
    public IActionResult Off(string mac)
    {
        _iotManager.LightOff(mac);
        return Ok(new { Msg = "success" });
    }
}

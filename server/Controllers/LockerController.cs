using BinHome.Server.Infrastructure;
using Microsoft.AspNetCore.Mvc;

namespace BinHome.Server.Controllers;

[Route("/locker")]
public class LockerController : ControllerBase
{
    private readonly IOTManager _iotManager;

    public LockerController(IOTManager iotManager)
    {
        _iotManager = iotManager;
    }

    [HttpPost("opendoor/{mac}")]
    public IActionResult OpenDoor(string mac)
    {
        _iotManager.OpenDoor(mac);
        return Ok(new { Msg = "success!" });
    }
}

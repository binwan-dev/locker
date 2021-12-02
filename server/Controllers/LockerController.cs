using BinHome.Server.Handlers;
using Microsoft.AspNetCore.Mvc;

namespace BinHome.Server.Controllers;

[Route("/locker")]
public class LockerController : ControllerBase
{
    private readonly LockerHandler _lockerHandler;

    public LockerController(LockerHandler lockerHandler)
    {
        _lockerHandler = lockerHandler;
    }

    [HttpPost("opendoor")]
    public IActionResult OpenDoor()
    {
        _lockerHandler.OpenDoor();
        return Ok(new { Msg = "open success!" });
    }
}
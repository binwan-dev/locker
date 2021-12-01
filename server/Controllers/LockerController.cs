using Microsoft.AspNetCore.Mvc;

namespace BinHome.Server.Controllers;

[Route("/locker")]
public class LockerController : ControllerBase
{
    [HttpPost("opendoor")]
    public Task<IActionResult> OpenDoor()
    {

    }
}
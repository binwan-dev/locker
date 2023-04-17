using BinHome.Server.Infrastructure;
using Microsoft.AspNetCore.Mvc;

namespace BinHome.Server.Controllers;

[Route("device")]
public class DeviceController : ControllerBase
{
    private readonly IOTManager _iocManager;

    public DeviceController(IOTManager iocManager)
    {
        _iocManager = iocManager;
    }

    [HttpGet]
    public IActionResult Get()
    {
        var devices = new List<object>();
        foreach (var device in _iocManager.Devices)
        {
            devices.Add(new
            {
                Mac = device.MacAddress,
                Type = device.Type.ToString(),
                Connected = device.Connection.Connected ? "Up" : "Down"
	    });
	}
        return Ok(devices);
    }
}

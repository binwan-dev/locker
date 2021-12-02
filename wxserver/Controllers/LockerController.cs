using Microsoft.AspNetCore.Mvc;

namespace BinHome.WxServer.Controllers
{
    [Route("locker")]
    public class LockerController : ControllerBase
    {
        [HttpPost("opendoor")]
        public async Task OpenDoorAsync()
        {
            using (var client = new HttpClient())
            {
                client.BaseAddress = new Uri("http://8.217.197.89:8003/");
                var resp = await client.PostAsync("/locker/opendoor", new StringContent(""));
                HttpContext.Response.StatusCode = (int)resp.StatusCode;
                HttpContext.Response.Body = await resp.Content.ReadAsStreamAsync();
            }
        }
    }
}
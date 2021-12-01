// index.js
// const app = getApp()

Page({

  onClickOpenDoor(e) {
    wx.request({
      url: 'https://locker-1444726-1308594886.ap-shanghai.run.tcloudbase.com/locker/opendoor',
      header: {
        'Content-Type': 'application/json'
      },
      success: function (data) {

      }
    })
  }
});

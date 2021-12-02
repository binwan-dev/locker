// index.js
// const app = getApp()

Page({

  onClickOpenDoor(e) {
    wx.request({
      url: 'https://locker-1444726-1308594886.ap-shanghai.run.tcloudbase.com/locker/opendoor',
      method: 'POST',
      header: {
        'Content-Type': 'application/json'
      },
      success: function (data) {
        if(data.statusCode==200){
          wx.showToast({
            title: '开门成功',
          })
        }else{
          wx.showToast({
            title: '开门失败',
          })
        }
      }
    })
  }
});

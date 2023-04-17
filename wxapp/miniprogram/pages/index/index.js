// index.js
// const app = getApp()

Page({
  data:{
    isBtnDisabled:false
  },
  async onClickOpenDoor(e) {
    this.setData({isBtnDisabled:true})
    let t=this
    await wx.cloud.callContainer({
      "config": {
        "env": "prod-0g1fqpmo2de33070"
      },
      "path": "/locker/opendoor",
      "header": {
        "X-WX-SERVICE": "locker",
        "content-type": "application/json"
      },
      "method": "POST",
      "data": ""
    })
    this.setData({isBtnDisabled:false})
    return
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
        t.setData({isBtnDisabled:false})
      },
      fail: function(e){
        wx.showToast({
          title: '开门失败',
        })
        t.setData({isBtnDisabled:false})
      }
    })
  }
});

# -*-coding=utf-8-*-
# 作者：天如
# 链接：http://www.zhihu.com/question/20341965/answer/14827914
# 来源：知乎
# 著作权归作者所有，转载请联系作者获得授权。

import wx
class ScrollbarFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, -1, 'Scrollbar Example',
        size=(400, 300))
        self.scroll = wx.ScrolledWindow(self, -1)
        self.scroll.SetScrollbars(1, 1, 600, 400)

        img1 = wx.Image("/home/yangzheng/myProgram/test/crow0.jpg", wx.BITMAP_TYPE_ANY)
        self.sb1 = wx.StaticBitmap(self.scroll,2, wx.BitmapFromImage(img1), pos=(40, 100))
        self.button = wx.Button(self.scroll, -1, "Scroll Me", pos=(50, 20))
        self.Bind(wx.EVT_BUTTON, self.OnClickTop, self.button)
        self.button2 = wx.Button(self.scroll, -1, "Scroll Back", pos=(500, 350))
        self.Bind(wx.EVT_BUTTON, self.OnClickBottom, self.button2)

    def OnClickTop(self, event):
        self.scroll.Scroll(1000, 900)

    def OnClickBottom(self, event):
        self.scroll.Scroll(1, 1)

if __name__ == '__main__':
    app = wx.PySimpleApp()
    frame = ScrollbarFrame()
    frame.Show()
    app.MainLoop()
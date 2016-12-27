import wx

class DrawPanel(wx.Frame):

    """Draw a line to a panel."""

    def __init__(self):
        wx.Frame.__init__(self, parent=None, title="Draw on Panel")
        temp = wx.Image("/home/yangzheng/myProgram/test/crow1.jpg", wx.BITMAP_TYPE_ANY)
        self.Bmp = temp.ConvertToBitmap()
        #self.BmpWin = wx.StaticBitmap(parent=self, bitmap=self.Bmp)
        #size = self.BmpWin.GetClientSize()
        size = wx.Size(100,100)
        self.showBmp = wx.EmptyBitmap(size.width, size.height)
        # print self.BmpWin
        self.Bind(wx.EVT_PAINT, self.OnPaint)

    def OnPaint(self, event=None):
        dc = wx.BufferedDC(wx.ClientDC(self), self.Bmp)
        #dc.Clear()
        dc.SetPen(wx.Pen(wx.BLACK, 4))
        dc.DrawLine(0, 0, 100, 100)

        self.Refresh()

app = wx.App(False)
frame = DrawPanel()
frame.Show()
app.MainLoop()
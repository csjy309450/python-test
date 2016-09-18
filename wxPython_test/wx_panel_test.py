#! /usr/bin/python2.7
# -*- coding=utf-8 -*-

import wx
import wx.py.images as img

class mainFrame(wx.Frame):
    def __init__(self, parent=None, id=-1,title='atest', pos=wx.DefaultPosition, size=wx.DefaultSize, style=wx.DEFAULT_FRAME_STYLE):
        wx.Frame.__init__(self, parent, id, title, pos, size, style)
        self.panel = panel = wx.Panel(self)
        self.panel.SetBackgroundColour("Red")

        # 创建状态栏
        statubar = self.CreateStatusBar()

        # 创建工具栏
        toolbar = self.CreateToolBar()
        toolbarItem = toolbar.AddSimpleTool(-1, img.getPyBitmap('PySlices'), 'New', "help for New")
        toolbar.AddSimpleTool(-1, img.getPyBitmap(), 'Edit', "help for Edit")
        toolbar.Realize()

        #创建菜单栏
        menubar = wx.MenuBar()
        menu1 = wx.Menu()
        menuItem = menu1.Append(-1, '&Save', 'Save help')
        menubar.Append(menu1, '&File')
        menu2 = wx.Menu()
        menu2.Append(wx.NewId(), "&Copy", "Copy in status bar")
        menu2.Append(wx.NewId(), "C&ut", "")
        menu2.Append(wx.NewId(), "Paste", "")
        menubar.Append(menu2, '&Edit')
        self.SetMenuBar(menubar)


        self.button = wx.Button(panel, label='button', pos=(0, 0), size=(100,40))

        self.Bind(wx.EVT_BUTTON, self.On_Button_Click, self.button)
        self.Bind(wx.EVT_MENU, self.On_Left_up, toolbarItem)

    def GetMenuItem(self):
        return (('&File', ('&Save', '&Save as')), ('&Edit', ('&Copy', '&Undo')))

    def On_Button_Click(self, event):
        # wx.MessageBox('Clicked the button')
        dlg = wx.MessageDialog(self, u"消息对话框", u"标题", wx.YES_NO|wx.ICON_QUESTION)
        if dlg.ShowModal() == wx.ID_YES:
            self.Close(True)
        dlg.Destroy()

    def On_Left_up(self, event):
        dlg = wx.TextEntryDialog(self, u"请在一下文本框中输入文本", u'标题', u'默认内容')
        str = u''
        if dlg.ShowModal() == wx.ID_OK:
            str = dlg.GetValue()
            dlg.Close()
        print str

class myApp(wx.App):
    def OnInit(self):
        self.Bmp = wx.Image("/home/yangzheng/myProgram/test/crow1.jpg", wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        self.mainWin = mainFrame()
        self.SetTopWindow(self.mainWin)
        self.mainWin.Show()
        return True

def main():
    app = myApp()
    app.MainLoop()


if __name__ == '__main__':
    main()

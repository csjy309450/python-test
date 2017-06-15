//
// Created by yangzheng on 17-6-15.
//

#ifndef GST_VIDEO_PLAYER_MYQFORM_H
#define GST_VIDEO_PLAYER_MYQFORM_H

#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include "ui_mydialog.h"
#include "gst_player.h"

class MyDialog : public QDialog, private Ui::MyDialog
{
Q_OBJECT
public:
    explicit MyDialog(int _argc, char* _argv[], QWidget *parent = 0);
    ~MyDialog();

public slots:
    void cli();

private:
    int argc;
    char **argv;
};


#endif //GST_VIDEO_PLAYER_MYQFORM_H

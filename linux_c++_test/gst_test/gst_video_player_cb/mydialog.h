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
class MyDialog : public QDialog, private Ui::MyDialog
{
Q_OBJECT
public:
    MyDialog(QWidget *parent = 0);
    ~MyDialog();

public slots:
    void cli();
};


#endif //GST_VIDEO_PLAYER_MYQFORM_H

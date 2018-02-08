//
// Created by yangzheng on 17-6-15.
//

#include "mydialog.h"


MyDialog::MyDialog(QWidget *parent)
{
    setupUi(this);
    QObject::connect(pushButton,SIGNAL(clicked()),this,SLOT(cli()));
}

MyDialog::~MyDialog()
{

}
void MyDialog::cli()
{
    QMessageBox::information(0,"aaa","bbb",QMessageBox::Yes|QMessageBox::No);
}
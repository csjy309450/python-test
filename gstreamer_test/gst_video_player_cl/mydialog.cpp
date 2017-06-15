//
// Created by yangzheng on 17-6-15.
//
#include "mydialog.h"


MyDialog::MyDialog(int _argc, char** _argv, QWidget *parent = 0):QDialog(parent),argc(_argc),argv(_argv)
{
    setupUi(this);
    QObject::connect(pushButton,SIGNAL(clicked()),this,SLOT(cli()));
}

MyDialog::~MyDialog()
{

}
void MyDialog::cli()
{
//    QMessageBox::information(0,"aaa","bbb",QMessageBox::Yes|QMessageBox::No);
    play(argc,argv,this->handle());
}
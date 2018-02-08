#ifndef DLG_SORT_H
#define DLG_SORT_H

#include <cstddef>
#include <iostream>
#include "ui_dlg_sort.h"

using namespace std;

class dlg_sort
    :public QDialog, public Ui_dlg_sort
{
Q_OBJECT
public:
    dlg_sort(QDialog * parent=NULL)
        : QDialog(parent)
    {
        setupUi(this);
        QObject::connect(pushButton_3, SIGNAL(toggled(bool)), groupBox_2, SLOT(setVisible(bool)));
        QObject::connect(pushButton_3, SIGNAL(toggled(bool)), this, SLOT(getState()));
        QObject::connect(pushButton_3, SIGNAL(toggled(bool)), groupBox_3, SLOT(setVisible(bool)));
        QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), this, SLOT(reject()));
    }
    ~dlg_sort(){}

private slots:
    void getState(){
        std::cout<<pushButton_3->isChecked();
    }
};

class run_app
{
public:
    run_app(int argc, char **argv){
        app=new QApplication(argc, argv);
        main_wid = new dlg_sort();
        main_wid->show();
        run_state = app->exec();
    }
    ~run_app(){
        main_wid->close();
        app->closeAllWindows();
        delete main_wid;
        delete app;
    }
    int get_state(){return run_state;}
private:
    QApplication *app;
    dlg_sort *main_wid;
    int run_state;
};

#endif //DLG_SORT_H
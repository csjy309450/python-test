#include "mydialog.h"
//#include "gst_player.h"


int main(int argc, char *argv[]) {
    QApplication app (argc, argv);
    MyDialog *my = new MyDialog(argc,argv);
    my->show();
    return app.exec();
//    play(argc,argv);
//    return 0;
}

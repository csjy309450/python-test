#include "my_player.h"

run_player::run_player(int argc, char *argv[])
{
	QApplication app(argc, argv);
	widget_player player(argc,argv);
	player.show();
	state = app.exec();
}

dlg_ctl::dlg_ctl(widget_player *_parent)
	: QDialog(_parent), parent(_parent)
{
	setupUi(this);
	QObject::connect(pushButton,SIGNAL(clicked()),parent,SLOT(cli_play()));
}

void dlg_ctl::cli_play(){
	//filter_play(argc,argv,parent->get_player_handle());
}

widget_player::widget_player(int _argc, char* _argv[], QWidget *_parent)
    :argc(_argc), argv(_argv), QWidget(_parent)
{
	setupUi(this);
	qdlg_ctl = new dlg_ctl(this);
	qdlg_ctl->show();
}

widget_player::~widget_player(){
	delete qdlg_ctl;
}

unsigned long widget_player::get_player_handle(){
	return this->winId();
}

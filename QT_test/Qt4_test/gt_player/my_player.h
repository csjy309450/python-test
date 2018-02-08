#ifndef MY_PLAYER_H
#define MY_PLAYER_H

#include <cstddef>
#include "ui_player.h"
#include "ui_dlg_ctl.h"

class run_app;
class widget_player;
class dlg_ctl;

class run_player{
public:
	run_player(int argc, char **argv);
	~run_player(){};
	inline int get_state(){return state;}

private:
	int state;
};

class dlg_ctl
	: public QDialog, public Ui_Dialog
{
Q_OBJECT
public:
	dlg_ctl(widget_player *_parent);
	~dlg_ctl(){};
private slots:
	void cli_play();
private:
	QWidget *parent;
};

class widget_player
	: public QWidget, public Ui_Form
{
Q_OBJECT
public:
	widget_player(int _argc, char* _argv[], QWidget *_parent = NULL);
	~widget_player();
	unsigned long get_player_handle();

private:
	dlg_ctl *qdlg_ctl;
	
    int argc;
    char **argv;
};

#endif //MY_PLAYER_H

#include "my_player.h"

int main(int argc, char** argv){
	run_player app(argc, argv);
	return app.get_state();
}

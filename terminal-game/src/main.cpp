#include "game.h"
 
int main(int argv, char** argc){
	int init_status = init();

	if(init_status == 0)
		run();

	close();
	return 0;
}

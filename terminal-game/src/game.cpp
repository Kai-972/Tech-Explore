#include<string>
#include<ncurses.h>
#include<unistd.h>

#include "game.h"

WINDOW* wnd;

struct {
	vec2i pos;
	char disp_char;
} player;

int init(){
	wnd = initscr();
	cbreak();
	noecho();
	clear();
	refresh();
	keypad(wnd, true);
	nodelay(wnd, true);
	curs_set(0);	// make the cursor invisible

	// check if terminal supports color manipulation
	if(!has_colors()){
		endwin();
		printf("ERROR: Terminal does not support color.\n");
		exit(1);
	}

	start_color();

	attron(A_BOLD);		// activate an attribute ( here bold type)
	box(wnd, 0, 0);		// used to draw frame around the window
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	wbkgd(wnd, COLOR_PAIR(1));
	attroff(A_BOLD);

	return 0;
}

void run() {

	player.disp_char = '0';
	player.pos = {10, 5};

	int in_char;

	bool exit_requested = false;

	while(1){
		in_char = wgetch(wnd);
		mvaddch(player.pos.y, player.pos.x, ' ');

		switch(in_char){
			case 'q':
				exit_requested = true;
				break;
			case KEY_UP:
			case 'w':
				player.pos.y -=1;
				break;
			case KEY_DOWN:
			case 's':
				player.pos.y +=1;
				break;
			case KEY_LEFT:
			case 'a':
				player.pos.x -=1;
				break;
			case KEY_RIGHT: 
			case 'd':
				player.pos.x +=1;
				break;
			default:
				break;
		}

		mvaddch(player.pos.y, player.pos.x, player.disp_char);
		refresh();

		if(exit_requested) break;

		usleep(10000);

		refresh();
	}
	
	// mvaddch(player.pos.y, player.pos.x, player.disp_char);
	
	// refresh();

	while(1);
}

void close() {
	endwin();
}

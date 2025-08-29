#include<string>
#include<ncurses.h>

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
	
	mvaddch(player.pos.y, player.pos.x, player.disp_char);
	refresh();

	while(1);
}

void close() {
	endwin();
}

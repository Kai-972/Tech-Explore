#include<unistd.h>
#include<ncurses.h>
#include<string>
#include<cstdint>
#include<vector>
#include<ctime>
#include<cmath>

#include "game.h"
#include "ObjectField.h"

WINDOW* main_wnd;
WINDOW* game_wnd;

rect game_area;
rect screen_area;

vec2ui cur_size;

ObjectField asteroids;
ObjectField stars;

struct {
	vec2i pos;
	rect bounds;
	char disp_char;
	int energy;
} player;

// Intro function
void show_intro() {
    int in_char = 0;
    int tick = 0;
 
    const std::vector<std::string> story_text = {
        "Just another Monday, and you're on your way to find job...",
        "When suddenly...",
        "You realize you left the job application from at home",
        "Take a shortcut through that asteroid field!",
        "Get back to the house before someone else take your job!"
    };
    
    mvwprintw(main_wnd, 22, 57, "press SPACE to skip...");

    size_t story_part = 0;
    size_t story_position = 0;

     while(1) {
        werase(game_wnd);
        in_char = wgetch(main_wnd);
        
        if(tick % 50 == 0)
            stars.update();

        for(auto s : stars.getData())
            mvwaddch(game_wnd, s.getPos().y, s.getPos().x, '.');
            
        if(story_position < story_text[story_part].length()) {
            wattron(main_wnd, A_BOLD);
            mvwaddch(main_wnd, 20, 5 + story_position, story_text[story_part][story_position]);
            wattroff(main_wnd, A_BOLD);
            story_position++;
        }

        if(in_char == ' ') {
            story_part++;
            story_position = 0;
            // CONCISE COMMENT: Clear the story line before writing the next part.
            mvwhline(main_wnd, 20, 1, ' ', screen_area.width() - 2);
        } else if(in_char == 'q') {
            exit(0); // Exit the program entirely if 'q' is pressed in intro
        }

        if(story_part >= story_text.size()) break;

        wrefresh(main_wnd);
        wrefresh(game_wnd);

        tick++;
        usleep(10000);
    }
}


int init(){
	
	srand(time(0));

	main_wnd = initscr();
	cbreak();
	noecho();
	clear();
	refresh();

	curs_set(0);	// make the cursor invisible

	start_color();

	screen_area = { {0,0}, {80, 24}};

	int infopanel_height = 4;
	game_wnd = newwin(
			screen_area.height() - infopanel_height - 2,
			screen_area.width() - 2,
			screen_area.top() + 1,
			screen_area.left() + 1
			);
	// main_wnd = newwin(screen_area.height(), screen_area.width(), 0, 0);
	game_area = { {0,0}, { screen_area.width() - 2, screen_area.height() - infopanel_height - 4}};

	// color pairs
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);

	keypad(main_wnd, true);
	keypad(game_wnd, true);

	nodelay(main_wnd, true);
	nodelay(game_wnd, true);

	// check if terminal supports color manipulation
	if(!has_colors()){
		endwin();
		printf("ERROR: Terminal does not support color.\n");
		exit(1);
	}

	start_color();

	return 0;
}


void run() {
    asteroids.setBounds(game_area);
    stars.setBounds(game_area);

    show_intro();

    // Outer loop to restart the game
    bool play_again = true;
    while (play_again) {

        //  Variables
	int tick = 0;
        player.disp_char = 'o';
        player.pos = {10, 5};
        player.energy = 100; // player energy

        asteroids.clear(); // Clear asteroids from the previous game
	stars.clear();
	
        int in_char;
        bool exit_requested = false;
        bool game_over = false;

	int score = 0;
        int timer_seconds = 0;

	// draw all static elements ONCE before the loop
	werase(main_wnd); // Clear the whole screen once
        box(main_wnd, 0, 0);
        wmove(main_wnd, game_area.bot() + 3, 1);
        whline(main_wnd, '-', screen_area.width() - 2);
        // Draw the static labels
        mvwprintw(main_wnd, game_area.bot() + 4, 2, "Time: ");
        mvwprintw(main_wnd, game_area.bot() + 4, 20, "Score: ");

        // main game loop
        while(!exit_requested && !game_over) {
            werase(game_wnd);
            
            in_char = wgetch(main_wnd);
            in_char = tolower(in_char);
            
            // player movement 
	    switch(in_char){
                case 'q': exit_requested = true; break;
                case KEY_UP: case 'w': case 'i':
                    if(player.pos.y > game_area.top()) player.pos.y -= 1;
                    break;
                case KEY_DOWN: case 's': case 'k':
                    if(player.pos.y < game_area.bot() + 1) player.pos.y += 1;
                    break;
                case KEY_LEFT: case 'a': case 'j':
                    if(player.pos.x > game_area.left() + 1) player.pos.x -=1;
                    break;
                case KEY_RIGHT: case 'd': case 'l':
                    if(player.pos.x < game_area.right() - 2) player.pos.x +=1;
                    break;
            }

            // game logic
            if(tick % 7 == 0) stars.update();
            if(tick > 200 && tick % 20 == 0) asteroids.update();
            
            // Update timer and score every second (approx).
            if (tick % 100 == 0) {
                timer_seconds++;
                score += 10; // 10 points for every second survived
            }

            // draw stars adn asteroids
            for(auto s: stars.getData()) mvwaddch(game_wnd, s.getPos().y, s.getPos().x, '.');
            for(auto a: asteroids.getData()) {
                wattron(game_wnd, A_BOLD);
                mvwaddch(game_wnd, a.getPos().y, a.getPos().x, '*');
                wattroff(game_wnd, A_BOLD);
            }

            // draw player ship
            wattron(game_wnd, A_BOLD);
            mvwaddch(game_wnd, player.pos.y, player.pos.x, player.disp_char);
            wattron(game_wnd, A_ALTCHARSET);
            mvwaddch(game_wnd, player.pos.y, player.pos.x - 1, ACS_LARROW);
            mvwaddch(game_wnd, player.pos.y, player.pos.x + 1, ACS_RARROW);
            wattroff(game_wnd, A_ALTCHARSET);

            if((tick % 10) / 3) {
                wattron(game_wnd, COLOR_PAIR(tick % 2 ? 3 : 4));
                mvwaddch(game_wnd, player.pos.y + 1, player.pos.x, ACS_UARROW);
                wattroff(game_wnd, COLOR_PAIR(tick % 2 ? 3 : 4));
            }

            // collision
            player.bounds = { { player.pos.x - 1, player.pos.y }, { 3, 1 } };
            for(size_t i = 0; i < asteroids.getData().size(); i++) {
                if(player.bounds.contains(asteroids.getData().at(i).getPos())) {
                    // BOOM A collision happened, end the game!
                    game_over = true;
                    flash(); // screen flash on impact
                }
            }

            // Draw UI / Info Panel
            mvwprintw(main_wnd, game_area.bot() + 4, 2, "Time: %d", timer_seconds);
            mvwprintw(main_wnd, game_area.bot() + 4, 20, "Score: %d", score);

            // Refresh and Delay								   	  
            wrefresh(main_wnd);
            wrefresh(game_wnd);

            tick++;
            usleep(10000);
        }

        // Game Over Screen
        if (game_over) {
            werase(main_wnd);
            box(main_wnd, 0, 0);
            mvwprintw(main_wnd, 10, 35, "GAME OVER");
            mvwprintw(main_wnd, 12, 32, "Final Score: %d", score);
            mvwprintw(main_wnd, 14, 26, "Press 'r' to Restart or 'q' to Quit");
            wrefresh(main_wnd);
            
            nodelay(main_wnd, false); // waiting for key press
            do {
                in_char = wgetch(main_wnd);
            } while (in_char != 'r' && in_char != 'q');

            if (in_char == 'q') {
                play_again = false;
            }
            nodelay(main_wnd, true); // Go back to non-blocking input for the next game
        } else {
            // If the loop was exited by pressing 'q'
            play_again = false;
        }
    }
}


void close() {
	endwin();
}

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define WIDTH 30
#define HEIGHT 9 

int startx = 0;
int starty = 0;

char choices[4][20] = { 
			"Play",
			"Recently Played",
			"Playlists",
			"Exit",
		  };
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);

int main()
{	WINDOW *menu_win;
	int highlight = 1;
	int choice = 0;
	int c, row, col;
	bool pause_id = false;
	char pid[10];
	char pause[20];
	FILE *fp;

	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	start_color();
	init_pair(1, 4, 0);
	startx = (80 - WIDTH) / 2;
	starty = (24 - HEIGHT) / 2;
	getmaxyx(stdscr,row,col);
		
	player: menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	attron(COLOR_PAIR(1) | A_BOLD);
	mvprintw(0, (col-70)/2 , "----- Welcome to Fast & Light Console-based (FaLiC) Media Player -----");
	attroff(COLOR_PAIR(1) | A_BOLD);
	mvprintw(3, 0, "Use arrow keys to go up and down, Press enter to select a choice");
	refresh();
	print_menu(menu_win, highlight);
	while(1)
	{	c = wgetch(menu_win);
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}	
	if (choice == 1) {
		system("./CreatePlaylist.sh");
		system("./playlist");
		clear();
		refresh();
		choice = 0;
		goto player;
	}
	else if (choice == 2) {
		system("./playlist");
		clear();
		refresh();
		choice = 0;
		goto player;
	}
	else if (choice == 3)
	{
		system("cp $(zenity --file-selection --title=\"Select Playlist\" --filename=/home/ashish/Desktop/Music_Player/FILES/Playlists) currentplay.txt");
		system("./playlist");
		choice=0;
		goto player;
	}
	else
	{
		system("killall mpg123 2>/dev/null");
	}
	clrtoeol();
	refresh();
	endwin();
	return 0;
}


void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	

	x = 2;
	y = 2;
	box(menu_win, 0, 0);
	for(i = 0; i < n_choices; ++i)
	{	if(highlight == i + 1) /* High light the present choice */
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}
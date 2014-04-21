#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define WIDTH 30
#define HEIGHT 9 

int startx = 0;
int starty = 0;

char choices[5][10] = { 
			"Play",
			"Pause",
			"Stop",
			"Playlist",
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
				//mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
				//getch();
				refresh();
				break;
		}
		print_menu(menu_win, highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
	}	
	if (choice == 1) {
		system("./mp3.sh");
		system("./playlist");
		clear();
		refresh();
		choice = 0;
		goto player;
	}
	else if (choice == 2) {
		fp = popen("pidof mpg123", "r");
		if (fp==NULL){
			printf("No song to pause!");
		}
		else {	
			fgets(pid, 10, fp);
			if (!pause_id){
				sprintf(pause, "kill -STOP %s", pid);
				strcpy(choices[1], "Resume");
				system(pause);
				pause_id = true;
			}
			else {
				sprintf(pause, "kill -CONT %s", pid);
				strcpy(choices[1], "Pause");	
				system(pause);
				pause_id = false;
			}
			choice = 0;
			goto player;
		}
		pclose(fp);
	}
	else if (choice == 3) {
		system("killall mpg123");
		choice = 0;
		goto player;
	}
	else if (choice == 4) {
		system("./playlist");
		clear();
		refresh();
		choice = 0;
		goto player;
	}
	//mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
	//getch();
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
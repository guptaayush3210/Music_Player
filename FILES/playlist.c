#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#define _GNU_SOURCE
#define WIDTH 70
#define HEIGHT 20

int startx = 0;
int starty = 0;

char choices[50][80];
int n_choices,curr_song=-1;
void print_menu(WINDOW *menu_win, int highlight);

void play_song(int item) {
	int i=0,j=0;
	char song[100]="mpg123 -q /host/ubuntu/songs/";
	char commandsong[100];
	system("killall mpg123 2>/dev/null");
	while(choices[item][j]!='\0') {
		if (choices[item][j] == ' ' || choices[item][j] == '(' || choices[item][j] == ')' || choices[item][j] == '\'')
			{commandsong[i++]='\\';commandsong[i]=choices[item][j];}
		else
			commandsong[i]=choices[item][j];
		j++;i++;
	}i--;
	//for(;i<100;i++)
		commandsong[i]='\0';
	// printw("%s",commandsong);
	// refresh();
	// strcat(song,commandsong);
	for(i=0;i<strlen(commandsong);i++)
	{
		song[i+29]=commandsong[i];
	}
	song[i+29]=' ';
	i++;
	song[i+29]='&';
	i++;
	//for(;i+29<100;i++)
		song[i+29]='\0';
	// printw("%s",song);
	system(song);
}

int main() {	
WINDOW *menu_win;
int highlight = 1;
int choice = -1;
int c=-1,z=0,i=0,j=0;
int inst;
char *line;
char *buff;
char read;
// int z=0;
/////////////////////////////////////////////////// FILE START ///////////////////////////////////////////////////////////
FILE *fp, *pstat;
   
   // char *choices[80];
   // char *s = "exit";
   
   fp = fopen("text.txt", "r");
   while(!feof(fp))
   {
   	buff = (char *)malloc(80*sizeof(char));
   fgets(buff,80,fp);
   // printw("%s", buff );
   strcpy(choices[z],buff);
   // printw("%s",choices[z]);
   z++;
   }
   // printw("%s %d",choices[z],z);
   n_choices = z;
   fclose(fp);
/////////////////////////////////////////////////// FILE END /////////////////////////////////////////////////////////////
initscr();
clear();
noecho();
start_color();
init_pair(1,4,0);
//halfdelay(1);
//cbreak();	/* Line buffering disabled. pass on everything */
startx = (80 - WIDTH) / 2;
starty = (24 - HEIGHT) / 2;
menu_win = newwin(HEIGHT, WIDTH, starty, startx);
keypad(menu_win, TRUE);

wtimeout(menu_win, 500);
mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
refresh();
p: print_menu(menu_win, highlight);
/*************************************************************************************************************/
while(1) {	
	c = wgetch(menu_win);
	switch(c)
	{	
		case -1: 		pstat = popen("ps -ewwo args | grep -c mpg123", "r");
						fscanf(pstat, "%d", &inst);
						if (inst == 2){
							if(curr_song < n_choices-3){
								curr_song++;
								play_song(curr_song);
								goto p;
							}
						}
						fclose(pstat);
						break;

		case KEY_UP:	if(highlight == 1)
						highlight = n_choices-1;
						else
						--highlight;
						break;
		case KEY_DOWN:	if(highlight == n_choices-1)
						highlight = 1;
						else
						++highlight;
						break;
		case 10:	choice = highlight;
					break;
		default:	//mvprintw(24, 0, "Characte/r pressed is = %3d Hopefully it can be printed as '%c'", c, c);
					//getch();
					refresh();
					break;
	}
	print_menu(menu_win, highlight);
	if(choice != -1)	/* User did a choice come out of the infinite loop */
	break;
}
/*************************************************************************************************************/	


/*--------------------------------------- SONG PLAYING STARTS----------------------------------------------------*/

if (choice < n_choices-1) {
	curr_song = choice-1;
	play_song(curr_song);
	choice=-1;
	goto p;
}

/*--------------------------------------- SONG PLAYING ENDS------------------------------------------------------*/

else if (choice == n_choices-1) {
	// system("killall mpg123 2>/dev/null");
	clear();
	choice=-1;
	return 0;
}
// mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
// getch();
clrtoeol();
refresh();
endwin();
return 0;
}


void print_menu(WINDOW *menu_win, int highlight) {
	int x, y, i=0;	

	x = 2;
	y = 2;
	// box(menu_win, 0, 0);
	for(i=0;i<n_choices;i++)
	{	
		start_color();
		init_pair(1,COLOR_CYAN, COLOR_BLACK);
		if(i == curr_song)  /* High light the present choice */
		{	
			if(highlight != i+1)
			{
			wattron(menu_win, A_BOLD | COLOR_PAIR(1));
			mvwprintw(menu_win, y, 0, "> %s",choices[i]);
			wattroff(menu_win, A_BOLD | COLOR_PAIR(1));
			}
			else
			{
				wattron(menu_win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
				mvwprintw(menu_win, y, 0, "> %s",choices[i]);
				wattroff(menu_win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
			}
		}
		else if (highlight == i+1)
		{
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, 0, "  %s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win, y, 0, "  %s", choices[i]);
			++y;
	}
	wrefresh(menu_win);
}
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#define WIDTH 100
#define HEIGHT 1

int startx = 0;
int starty = 0;

char *choices[] = {
"Now Playing",
"Add Songs",
"Pause",
"Stop",
"Next",
"Fast Forward",
"Rewind",
"Exit",
};
int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *play_menu, int highlight, int row);

int main()
{	WINDOW *play_menu;
int highlight = 1;
int choice = 0;
int c;
int row,col;
initscr();
clear();
noecho();
cbreak();	/* Line buffering disabled. pass on everything */
getmaxyx(play_menu,row,col);
startx = 0;
starty = 0;

play_menu = newwin(HEIGHT, WIDTH, starty, startx);
keypad(play_menu, TRUE);
// mvprintw(0, 0, "Use arrow keys to go left and right, Press enter to select a choice");
refresh();
player: print_menu(play_menu, highlight,row);
while(1)
{	c = wgetch(play_menu);
switch(c)
{	case KEY_LEFT:
if(highlight == 1)
highlight = n_choices;
else
--highlight;
break;
case KEY_RIGHT:
if(highlight == n_choices)
highlight = 1;
else
++highlight;
break;
case 10:
choice = highlight;
break;
default:
// mvprintw(24, 0, "Character pressed is = %3d Hopefully it can be printed as '%c'", c, c);
// getch();
refresh();
break;
}
print_menu(play_menu, highlight,row);
if(choice != 0)	/* User did a choice come out of the infinite loop */
break;
}	
if (choice == 1) {
system("./a.out");
choice=0;
goto player;
}
else if (choice == 2)
{
	system("./mp3.sh");
	choice = 0;
	goto player;
}
else if (choice == 3)
{
	choice =0;
	goto player;
}
else if (choice == 4) {
system("killall mpg123");
choice=0;
goto player;
}
else if (choice == 5)
{
	choice = 0;
	goto player;
}
// mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice - 1]);
getch();
clrtoeol();
refresh();
endwin();
return 0;
}


void print_menu(WINDOW *play_menu, int highlight, int row)
{
int x, y, i;
x = 0;
y = 0;
// box(play_menu, 0, 0);
for(i = 0; i < n_choices; ++i)
{	
if (i>0)
{
	x += strlen(choices[i-1])+2;
}
if(highlight == i + 1) /* High light the present choice */
{	wattron(play_menu, A_REVERSE);
mvwprintw(play_menu,y,x, "%s", choices[i]);
wattroff(play_menu, A_REVERSE);
}
else
mvwprintw(play_menu,y,x, "%s", choices[i]);

}
wrefresh(play_menu);
}
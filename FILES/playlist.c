#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#define WIDTH 70
#define HEIGHT 20


int startx = 0;
int starty = 0;

char *npchoices[] = {
"Add Songs",
"Pause",
"Stop",
"Next",
"Save Playlist",
"Main Menu",
};
int npn_choices = sizeof(npchoices) / sizeof(char *);
int row,col;
char *choices[50];
int n_choices,curr_song=-1;
void print_menu(WINDOW *menu_win, int highlight, int nphighlight);
void play_song();

int main()
{	
WINDOW *menu_win;
int highlight = 1,nphighlight=1;
int choice = -1,npchoice = 0;
int c,z=0,i=0,j=0;
char *line;
char *buff;
char read;
bool pause_id = false;
char *pid;
char pause[20];
FILE *f;
/////////////////////////////////////////////////// FILE START ///////////////////////////////////////////////////////////
 FILE *fp;   
   start: 
   z=0;
   fp = fopen("currentplay.txt", "r");
   while(!feof(fp))
   {
   	buff = (char *)malloc(200*sizeof(char));
   fgets(buff,80,fp);
   choices[z]=buff;
   z++;
   }
   n_choices = z;
   fclose(fp);
/////////////////////////////////////////////////// FILE END /////////////////////////////////////////////////////////////
initscr();
clear();
noecho();
cbreak();	/* Line buffering disabled. pass on everything */
startx = (80 - WIDTH) / 2;
starty = (24 - HEIGHT) / 2;
getmaxyx(stdscr,row,col);
menu_win = newwin(HEIGHT, WIDTH, starty, startx);
keypad(menu_win, TRUE);

/////////////////////////////////////////////////////////// PAINT BLACK //////////////////////////////////////////////////
start_color();
init_pair(2,COLOR_WHITE,COLOR_BLACK);
////////////////////////////////////////////////////////// PAINT BLACK //////////////////////////////////////////////////
mvprintw(0, 0, "Use arrow keys to go up and down, left and right, Press enter to select a song and space to selct lower menu choice.");
// mvprintw(1, 0, "%d %d",row,col);
refresh();
p:
print_menu(menu_win, highlight, nphighlight);
/*************************************************************************************************************/
while(1)
{	
	c = wgetch(menu_win);
	switch(c)
	{
		case KEY_UP:
			if(highlight == 1)
				highlight = n_choices-1;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if(highlight == n_choices-1)
				highlight = 1;
			else
				++highlight;
			break;
		case KEY_RIGHT:
			if (nphighlight == npn_choices)
				nphighlight = 1;
			else
				nphighlight++;
			break;
		case KEY_LEFT:
			if (nphighlight == 1)
				nphighlight = npn_choices;
			else
				nphighlight--;
			break;
		case 10:
			choice = highlight;
			break;
		case 32:
			npchoice = nphighlight;
			break;
		default:
			refresh();
			break;
	}
	print_menu(menu_win, highlight, nphighlight);
	if(choice != -1 || npchoice !=0)	/* User did a choice come out of the infinite loop */
	break;
}
/*************************************************************************************************************/	


/*--------------------------------------- SONG PLAYING START------------------------------------------------------*/

if (choice != -1)
{
	curr_song= choice-1;
	play_song(menu_win);
	choice=-1;
	goto p;
}

/*--------------------------------------- SONG PLAYING ENDS------------------------------------------------------*/

else
{
	if (npchoice == 1)
	{
		system("./AddSongs.sh");
		npchoice =0;
		goto start;
	}
	else if (npchoice == 2)
	{
			f = popen("pidof mpg123", "r");
			if (f==NULL){
				printf("No song to pause!");
			}
			else {
				fgets(pid, 10, f);
			if (!pause_id){
				sprintf(pause, "kill -STOP %s", pid);
				wprintw(menu_win,"%s",pause);
				strcpy(npchoices[1], "Resume");
				system(pause);
				pause_id = true;
			}
			else {
				sprintf(pause, "kill -CONT %s", pid);
				strcpy(npchoices[1], "Pause ");	
				system(pause);
				pause_id = false;
			}
			pclose(f);
			npchoice = 0;
			goto p;
			}
	}
	else if (npchoice == 3)
	{
		system("killall mpg123 2>/dev/null");
		npchoice=0;
		curr_song = -1;
		goto p;
	}
	else if (npchoice == 4)
	{
		if (curr_song == n_choices-2)
			curr_song = 0;
		else
			curr_song++;

		npchoice = 0;
		play_song(menu_win);
		goto p;
	}
	else if (npchoice == 5)
	{
		system("./SavePlaylist.sh");
		npchoice =0;
		goto p;
	}
	else if (npchoice == 7)
	{
		clear();
		choice = -1;
		echo();
		return 0;
	}
}
clrtoeol();
refresh();
endwin();
echo();
return 0;
}


void print_menu(WINDOW *menu_win, int highlight, int nphighlight)
{
	int x, y, i=0,z=0,start;
	char name[200];
	x = 0;
	y = 2;
	for(i=0;i<n_choices;i++)
	{	
		start_color();
		init_pair(1,COLOR_CYAN, COLOR_BLACK);
		for (z=0;z<strlen(choices[i]);z++)
		{
			if (choices[i][z]=='/')
				start =z+1;
		}
		for(z=0;z<200;z++)
		{
			while(choices[i][z]!='\0')
			{
				name[z]=choices[i][start++];
				z++;
			}
			name[z]='\0';
		}
		if(i == curr_song)  /* High light the present choice */
		{	
			if(highlight != i+1)
			{
				wattron(menu_win, A_BOLD | COLOR_PAIR(1));
				mvwprintw(menu_win, y, 0, "> %s",name);
				wattroff(menu_win, A_BOLD | COLOR_PAIR(1));
			}
			else
			{
				wattron(menu_win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
				mvwprintw(menu_win, y, 0, "> %s",name);
				wattroff(menu_win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
			}

		}
		else if (highlight == i+1)
		{
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, 0, "  %s", name);
			wattroff(menu_win, A_REVERSE);
		}
		else
				mvwprintw(menu_win, y, 0, "  %s", name);
		++y;
	}
	y+=6;
	for(i = 0; i < npn_choices; ++i)
	{	
		if (i>0)
		{
			x += strlen(npchoices[i-1])+2;
		}
		if(nphighlight == i + 1) /* High light the present choice */
		{	
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win,y,x, "%s", npchoices[i]);
			wattroff(menu_win, A_REVERSE);
		}
		else
			mvwprintw(menu_win,y,x, "%s", npchoices[i]);

}

wrefresh(menu_win);
}

void play_song(WINDOW *menu_win)
{
	int i=0,j=0;
	char song[100]="mpg123 -q ";
	char commandsong[100];
	system("killall mpg123 2>/dev/null");
	while(choices[curr_song][j]!='\0')
	{
		if (choices[curr_song][j] == ' ' || choices[curr_song][j] == '(' || choices[curr_song][j] == ')' || choices[curr_song][j] == '\'')
			{commandsong[i++]='\\';commandsong[i]=choices[curr_song][j];}
		else
			commandsong[i]=choices[curr_song][j];
		j++;
		i++;
	}
	i--;
	for(;i<100;i++)
		commandsong[i]='\0';
	for(i=0;i<strlen(commandsong);i++)
	{
		song[i+10]=commandsong[i];
	}
	song[i+10]=' ';
	i++;
	song[i+10]='&';
	i++;
	for(;i+10<100;i++)
		song[i+10]='\0';
	system(song);
}
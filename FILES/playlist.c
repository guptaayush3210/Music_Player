#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#define _GNU_SOURCE
#define WIDTH 90
#define HEIGHT 20

int startx = 0;
int starty = 0;

char npchoices[8][20] = {
"Add Songs",
"Pause ",
"Stop",
"Next",
"Repeat ON ",
"Shuffle ON ",
"Save Playlist",
"Main Menu",
};
int npn_choices = 8;
int row,col;
char choices[50][200];
int n_choices,curr_song=-1;
void print_menu(WINDOW *menu_win, int highlight, int nphighlight);

void play_song() {
	int i=0,j=0;
	char song[210]="mpg123 -q ";
	char commandsong[200];
	system("killall mpg123 2>/dev/null");
	while(choices[curr_song][j]!='\0') {
		if (choices[curr_song][j] == ' ' || choices[curr_song][j] == '(' || choices[curr_song][j] == ')' || choices[curr_song][j] == '\'')
			{commandsong[i++]='\\';commandsong[i]=choices[curr_song][j];}
		else
			commandsong[i]=choices[curr_song][j];
		j++;
		i++;
	}i--;
	for(;i<200;i++)
		commandsong[i]='\0';
	for(i=0;i<strlen(commandsong);i++)
	{
		song[i+10]=commandsong[i];
	}
	song[i+10]=' ';
	i++;
	song[i+10]='&';
	i++;
	for(;i+10<200;i++)
		song[i+10]='\0';
	system(song);
}

int main() {	
WINDOW *menu_win;
int highlight = 1,nphighlight=1;
int choice = -1,npchoice = 0;
int c=-1,z=0,i=0,j=0;
int inst;
char *line;
char *buff;
char read;
bool pause_id = false, stop_id = false,repeat = false, shuffle = false;
char pid[10];
char pause[21];
/////////////////////////////////////////////////// FILE START ///////////////////////////////////////////////////////////
FILE *fp, *pstat, *f;
   
start: 
z=0;
fp = fopen("currentplay.pl", "r");
while(!feof(fp)) {
	buff = (char *)malloc(200*sizeof(char));
	fgets(buff,150,fp);
	strcpy(choices[z], buff);
	z++;
}
n_choices = z;
fclose(fp);
/////////////////////////////////////////////////// FILE END /////////////////////////////////////////////////////////////
initscr();
clear();
noecho();
srand(time(NULL));
//cbreak();	/* Line buffering disabled. pass on everything */
startx = (100 - WIDTH) / 2;
starty = (24 - HEIGHT) / 2;
getmaxyx(stdscr,row,col);
menu_win = newwin(HEIGHT, WIDTH, starty, startx);
keypad(menu_win, TRUE);

/////////////////////////////////////////////////////////// PAINT BLACK //////////////////////////////////////////////////
start_color();
init_pair(2,COLOR_WHITE,COLOR_BLACK);
////////////////////////////////////////////////////////// PAINT BLACK ///////////////////////////////////////////////////

wtimeout(menu_win, 500);
mvprintw(0, 0, "Use arrow keys to go up and down, Press enter to select a choice");
refresh();
p: print_menu(menu_win, highlight, nphighlight);
/*************************************************************************************************************/
while(1) {	
	c = wgetch(menu_win);
	switch(c)
	{	
		case -1: 		pstat = popen("ps -ewwo args | grep -c mpg123", "r");
						fscanf(pstat, "%d", &inst);
						if (inst == 2 && !stop_id){
							if(shuffle){
								curr_song = rand()%(n_choices-2);
								play_song();
								goto p;
							}
							else if(curr_song < n_choices-2){
								curr_song++;
								play_song();
								goto p;
							}
							else if (repeat)
							{
								curr_song = 0;
								play_song();
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
		case KEY_RIGHT:	if (nphighlight == npn_choices)
							nphighlight = 1;
						else
							nphighlight++;
						break;
		case KEY_LEFT:	if (nphighlight == 1)
							nphighlight = npn_choices;
						else
							nphighlight--;
						break;
		case 10:	choice = highlight;
					break;
		case 32:	npchoice = nphighlight;
					break;
		default:	refresh();
					break;
	}
	print_menu(menu_win, highlight, nphighlight);
	if(choice != -1 || npchoice !=0)	/* User did a choice come out of the infinite loop */
	break;
}
/*************************************************************************************************************/	


/*--------------------------------------- SONG PLAYING STARTS----------------------------------------------------*/

if (choice != -1) {
	curr_song = choice-1;
	play_song();
	stop_id = false;
	choice=-1;
	goto p;
}

/*--------------------------------------- SONG PLAYING ENDS------------------------------------------------------*/

else {
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
				clear();
				refresh();
				goto p;
			}
	}
	else if (npchoice == 3)
	{
		system("killall mpg123 2>/dev/null");
		npchoice=0;
		stop_id = true;
		curr_song = -1;
		goto p;
	}
	else if (npchoice == 4)
	{
		if (shuffle)
			curr_song = rand()%(n_choices-2);
		else{
			if (curr_song == n_choices-2)
				curr_song = 0;
			else
				curr_song++;
		}
		npchoice = 0;
		play_song();
		stop_id = false;
		goto p;
	}
	else if (npchoice == 5)
	{
		if(!repeat){	
			repeat = true;
			sprintf(npchoices[4],"Repeat OFF");
		}
		else {
			repeat = false;
			sprintf(npchoices[4],"Repeat ON ");
		}
		npchoice =0;
		goto p;
	}
	else if (npchoice == 6)
	{
		if(!shuffle){	
			shuffle = true;
			sprintf(npchoices[5],"Shuffle OFF");
		}
		else {
			shuffle = false;
			sprintf(npchoices[5],"Shuffle ON ");
		}
		npchoice =0;
		goto p;
	}
	else if (npchoice == 7)
	{
		system("./SavePlaylist.sh");
		npchoice =0;
		goto p;
	}
	else if (npchoice == 8)
	{
		system("killall mpg123 2>/dev/null");
		clear();
		choice = -1;
		return 0;
	}
}
clrtoeol();
refresh();
endwin();
return 0;
}


void print_menu(WINDOW *menu_win, int highlight, int nphighlight)
{
	int x, y, i=0,z=0,start,start_point,end_point,tens;
	char name[200];
	x = 0;
	y = 2;
	wclear(menu_win);
	wrefresh(menu_win);
	tens = (highlight-1)/10;
	start_point = tens*10;
	if (start_point+10 <= n_choices-1)
		{end_point = start_point+10;}
	else
		{end_point = n_choices-1;}

	for(i=start_point;i<end_point;i++)
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
				mvwprintw(menu_win, y, 0, ">>   %s",name);
				wattroff(menu_win, A_BOLD | COLOR_PAIR(1));
			}
			else
			{
				wattron(menu_win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
				mvwprintw(menu_win, y, 0, ">>   %s",name);
				wattroff(menu_win, A_BOLD | A_REVERSE | COLOR_PAIR(1));
			}

		}
		else if (highlight == i+1)
		{
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, 0, "%.2d   %s",i+1, name);
			wattroff(menu_win, A_REVERSE);
		}
		else
				mvwprintw(menu_win, y, 0, "%.2d   %s",i+1, name);
		++y;
	}
	y=16;
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
#include <curses.h>	/* Necessary for all Curses programs */
#include <menu.h>		/* Gives you menuing capabilities */
#include "shell_int.h"

#define WHITEONRED 1
#define WHITEONBLUE 2
#define WHITEONBLACK 3
#define BLACKONWHITE 4
#define REDONWHITE 5

/*
BUFSIZE 256と ファイルの行数用のFILELINES 1024くらいのマクロ定義
あとはいろんなファイル読み込ませて, デバッグを行う
上にソースを送るとき画面外へいくと見えなくなる問題
WINDOWを画面外へださないようにする
*/

/* Prototype ******************************************************************************/
void wCenterTitle(WINDOW *pwin, const char * title);
void wclrscr(WINDOW * pwin);
bool initColors(void);
int runMenu( WINDOW *wParent, int height, int width, int y, int x, char *choices[], char *menubar);
int SetMenu(char **choices, int nest, int number);
int SourceWINDOW(int);
void strchg(char *buf, const char *str1, const char *str2);
void ConvertEnter( char *c);
extern void Data(char *fname);
extern int FILE_LINE(char *);
extern int Shell(char *);

/* Structure *****************************************************************************/
extern struct _result{
   int number;
   int nest ;
   char name[CHARACTER];
   struct _result *next;
   int start_line;
   int end_line;
   // struct _result *before;
 } result;

/* Global Variable **********************************************************************/
int SCREEN_COUNTER = 0;
int c;											                     /* store push key word */	
int nest = 1;
int select_number[BUFSIZE];
int stack_number[BUFSIZE];
char fileNAME[BUFSIZE] = {'\0'};
int sp = 1;
int global = 0;
int BeLINE = 0;
extern struct _result *p, *start;

/* Main function ******************************************************************************************************************/
int main(int argc, char *argv[]){

  FILE *fp;
  Shell(&fileNAME); // To use `cd' and 'ls' command   
	Data(fileNAME);   // To analyze file structure
	
	char *choices[256] = {};
	int i = 0;
	int n_choices = 0;
  int choiceno = 0;

  i = SetMenu(choices, 0 ,0);

  choices[i] = "\0";

  //Setting Ncurses preferences
	initscr();															// start ncurses 
  cbreak();																// immediately acquire each keystroke 
  noecho();																// do not echo user keystrokes 
	keypad(stdscr, TRUE);										// enable detection of function keys 
	initColors();														// enable colors and initialize pairs 

	// Set up and paint standard screen
	wattrset(stdscr, COLOR_PAIR(WHITEONBLUE) | A_BOLD);	
	wclrscr(stdscr);
	touchwin(stdscr);
	wrefresh(stdscr);

	// Acquire the user's choice
  choiceno = runMenu(stdscr, 16, 40, 2, 20, choices, "main");		//std, height, width, menu-start-point(height, width)

  // Quit Program message
	mvwaddstr(stdscr, 22, 0, "Shout Down _...\\\\");				
	touchwin(stdscr);
	wrefresh(stdscr);

  // End ncurses
  endwin();
  // free 
	free(p);
  
	return 0;
}

int SetMenu(char **choices, int nest, int number){

  BeLINE=number;
  p = start;

  int i = 0;
  int n = 0;
  int reg = 0;
  
  if( number == 0 && global != 0){
    choices[0] = "back";
    stack_number[0] = 0;
    i++;
    while (p != NULL) {
      if( stack_number[sp-2] == p->number ){
        break;
      }
      p = p->next;
    }
    nest--;nest--;
    number = p->number;
    BeLINE=number;
    p = start;
    goto a;

    while (p != NULL) {
      if( nest-1 == p->nest){
        if( stack_number[sp-2] <= p->number ){
          select_number[i] = p->number;
          choices[i] = p->name;
          i++;
        }
      }

      if(nest-2 == p->nest){
         break;
      }
      p = p->next;
    }
    return i;
  }
  
  // ----------------------------------------------------------

  global++;
  a:
  while (p != NULL) {
  	if( p->number == number )
  		break;
    if( p->nest == nest )
      reg = p->number;
  	p = p->next;
  }

  p = start;
  while (p != NULL) {
    if( p->number == reg )
      break;
    p = p->next;
  }
  
  if( nest == 0 ){
    choices[0] = "back";
    i = 1;
    p = p->next;  
    while (p != NULL) {
      if( nest+1 == p->nest){
        if( number < p->number ){
          select_number[i] = p->number;
          choices[i] = p->name;
          i++;
        }
      }
      if(nest == p->nest)
        break;
      p = p->next;
    }
    return i;
  }
  
  // -------------------------------------------------
  choices[0] = "back";
  i = 1;
  p = p->next;

  while (p != NULL) {
    if( nest+1 == p->nest){
    	if( number > p->number ){
        select_number[i] = p->number;
      	choices[i] = p->name;
      	i++;
    	}
    }
    if(nest == p->nest)
      break;
    p = p->next;
  }
  return i ;
}

void wCenterTitle(WINDOW *pwin, const char * title){
	int x, maxy, maxx, stringsize;

	stringsize = 4 + strlen(title);
  getmaxyx(pwin, maxy, maxx);
	x = (maxx - stringsize)/2;
	
  mvwaddch(pwin, 0, x, ACS_RTEE);
	waddch(pwin, ' ');
	waddstr(pwin, title);
	waddch(pwin, ' ');
	waddch(pwin, ACS_LTEE);
}

void wclrscr(WINDOW * pwin){
	int y, x, maxy, maxx;
	getmaxyx(pwin, maxy, maxx);
	for(y=0; y < maxy; y++)
		for(x=0; x < maxx; x++)
			mvwaddch(pwin, y, x, ' ');
}

bool initColors(void){
	if(has_colors()){
		start_color();
		init_pair(WHITEONRED, COLOR_BLACK, COLOR_WHITE);
    init_pair(WHITEONBLUE, COLOR_WHITE, COLOR_BLACK);
		init_pair(REDONWHITE, COLOR_RED, COLOR_WHITE);
		return(true);
	}
  else
		return(false);
}

int runMenu( WINDOW *wParent, int height, int width, int y, int x, char *choices[], char *menubar){

	ITEM **my_items;						// list of items on this menu 
	MENU *my_menu;							// the menu structure 

	WINDOW *wUI;								// window on which the user interacts with the menu 
	WINDOW *wBorder;						// window containing the wUI window and the border and title 

  int n_choices = 0;							// number of items on menu 
  int ssChoice;								// subscript to run around the choices array 
	int my_choice = -1;					// the zero based numeric user choice 
  char s1[BUFSIZE];               // use to cat and trance
  int i;

	// Calculate number of menu chices
	for(n_choices=0; choices[n_choices]; n_choices++){
		;
	}

  // Allocate item array and individual items
  my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
  for(ssChoice = 0; ssChoice < n_choices; ++ssChoice)
    my_items[ssChoice] = new_item(choices[ssChoice], NULL);
	my_items[n_choices] = (ITEM *)NULL;

  // Create the menu structure 
	my_menu = new_menu((ITEM **)my_items);

  // Put '>' to the left of highlighted item
	set_menu_mark(my_menu, "> ");

  // Set up window for menu's border
	wBorder = newwin(height, width, y, x);
	wattrset(wBorder, COLOR_PAIR(WHITEONRED) | WA_BOLD);
	wclrscr(wBorder); 
	box(wBorder, '|', '-');
	wCenterTitle(wBorder, menubar);

  // Set up WINDOW for the menu's user interface
	wUI = derwin(wBorder, height-2, width-2, 2, 2);

  // Associate these WINDOWS with the menu
	set_menu_win(my_menu, wBorder);
	set_menu_sub(my_menu, wUI);

  // Match menu's colors to that of its WINDOWS
	set_menu_fore(my_menu, COLOR_PAIR(REDONWHITE));
	set_menu_back(my_menu, COLOR_PAIR(WHITEONRED) | WA_BOLD);

  // Set up an environment conductive to menuing 
	keypad(wUI, TRUE);				// enable detection of function keys 
	noecho();									// user keystrokes don't echo 
	curs_set(0);							// make cursor invisible 

	// Display the menu 
	post_menu(my_menu);

  // Refresh the Border WINDOW priot to accepting user interaction 
	touchwin(wBorder);
	wrefresh(wBorder);

  // Handle user keystrokes 
	while(my_choice == -1){
		touchwin(wUI);					/* refresh prior to getch() */
		wrefresh(wUI); 					/* refresh prior to getch() */
    c = getch();

		switch(c){
      case KEY_RIGHT://display source of menu item  
        SourceWINDOW(BeLINE);
        break;

			case KEY_DOWN:
        menu_driver(my_menu, REQ_DOWN_ITEM);
				break;

			case KEY_UP:
        menu_driver(my_menu, REQ_UP_ITEM);
				break;

			case 10:	// '10' means 'Enter' key
				my_choice = item_index(current_item(my_menu));
				stack_number[sp] = select_number[my_choice];
        i = SetMenu(choices, nest, select_number[my_choice]);
        choices[i] = "\0";
				
        if( my_choice == 0 && SCREEN_COUNTER == 0)
          break;
        if( my_choice !=  0 ){
					SCREEN_COUNTER++; nest++; sp++;
        }else{
					if(SCREEN_COUNTER != 0)
						SCREEN_COUNTER--; nest--; sp--;
        }

				runMenu(stdscr, 16, 40, 2+SCREEN_COUNTER*2, 20+SCREEN_COUNTER*10, choices, choices[my_choice]);
				
        // Reset cursor in case more selection is necessary 
				pos_menu_cursor(my_menu);
				break;

			case 'q': // 'q' key push to quit program
				my_choice = 0;
				break ;
		}
	}	
	
  // Free all allocated menu and item resorces 
	unpost_menu(my_menu);
  for(ssChoice = 0; ssChoice < n_choices; ++ssChoice)
    free_item(my_items[ssChoice]);
	free_menu(my_menu);

  // Destroy menu WINDOW and border WINDOWS
	if(SCREEN_COUNTER == 0){
		delwin(wUI);
		delwin(wBorder);
	}	
	/* Undo menu specific environment 
	curs_set(1);			/* make cursor visible again */
	
	/* Repaint the calling screen in preparation for return */
	touchwin(wParent);
	wrefresh(wParent);

	// Return the zero based numeric user choice
	return(my_choice);
}


int SourceWINDOW(int number){
    int LINEstart = 0, LINEend = 0, LINEcoutner = 0;
    
    p = start;
    while (p != NULL) {
      if( p->number == number )
        break;
      p = p->next;
    }

    LINEstart = p->start_line;
    LINEend = p->end_line;

    WINDOW *base_win;
    int x, y;
    char *source[BUFSIZE];
    int key = 0;
    int counter;
    int i = 0;
    char buf[BUFSIZE]={0};
    FILE *fp;

    getmaxyx(stdscr, y, x);
    base_win = newwin(FILE_LINE(fileNAME)+4 , x/2, 0, x/2);
    keypad(base_win, TRUE);
    scrollok(base_win, TRUE);
    noecho();                 /* user keystrokes don't echo */
    curs_set(0);              /* make cursor invisible */
    wattrset(base_win, COLOR_PAIR(WHITEONBLUE) | WA_BOLD);
    wclrscr(base_win);
    box(base_win, '|', '-');
    wCenterTitle(base_win, "Source");


    if ((fp = fopen(fileNAME, "r")) == NULL) {
      fprintf(stderr, "%s is no such file or directory.\n", fileNAME);
      exit(EXIT_FAILURE);
    }

    while ( (source[i] = fgets(buf, BUFSIZE, fp)) != NULL ) {
      if(LINEstart-1 <= LINEcoutner && LINEend-1 >= LINEcoutner){
        strchg(source[i], "\t", "  ");
        mvwinsstr(base_win, 3+i, 2, source[i]);
        i++;
      }
      LINEcoutner++;
    }

    fclose(fp);

    touchwin(base_win);
    wrefresh(base_win);

    while(1){
      key = getch();
      switch(key){
        case KEY_LEFT:
          return 0;
          break;
        case KEY_DOWN:
          wscrl(base_win, 1);
          touchwin(base_win);
          wrefresh(base_win);
          break;
        case KEY_UP:
          wscrl(base_win, -1);
          touchwin(base_win);
          wrefresh(base_win);
          break;
        default:
          break;
      }
    }
    /*Display the small window */
    touchwin(base_win);
    wrefresh(base_win);
}

void strchg(char *buf, const char *str1, const char *str2){
  //https://www.grapecity.com/tools/support/powernews/column/clang/049/page03.htm
  char tmp[1024 + 1];
  char *p;
  while ((p = strstr(buf, str1)) != NULL) {   // 見つからなくなるまで繰り返すpは旧文字列の先頭を指している 
    *p = '\0';          // 元の文字列を旧文字列の直前で区切って 
    p += strlen(str1);  // ポインタを旧文字列の次の文字へ 
    strcpy(tmp, p);     // 旧文字列から後を保存 
    strcat(buf, str2);  // 新文字列をその後につなぎ 
    strcat(buf, tmp);   // さらに残りをつなぐ 
  }

}

void ConvertEnter( char *c){
  int i = 0;
  while( c[i] != '\n' )
    i++;
  c[i] = '\0';
}


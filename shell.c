/* ****************************** * 
 *	date : 2015/07/31  			      *
 *	name : Hanzawa JUNICHI        *
 *	abstruct : Lite shell         *
 * ****************************** */

/*preprocessor*/
#include "shell_int.h"

/*function prototype */
void exception(char *buf);
int exec(char **com, char *fileNAME);
void tok(char *buf, char **com);
void initialize(char *buf);

int Shell(char *fileNAME){
	
	char *com[BUFSIZE];		//it is store comand  
	char buf[BUFSIZE];		//separate input text
	int i = 0, j = 0;			//loop varialble
	char pwd[BUFSIZE];

	while(1){//loop

		initialize(buf);//initialize call

		i = 0;// initialize
		getcwd(pwd, BUFSIZE);
		printf("%s at %s\n", getenv("USER"), pwd);
		printf(" >> $ ");

		exception(buf);//call text maker
		
		int semicolon = 0;
		char *temp[BUFSIZE];	//saved ';' stiring
		char *semi_save;	//is used strtok_r
		
		/*number of ';'*/
		for( i = 0 ; i < BUFSIZE ; i++)
			if( buf[i] == ';')
				semicolon++;

		/*not empty text*/
		if(strcmp(&buf[0],"\0") != 0){
			/*if there is ';' in buf string*/
			if(semicolon > 0){
				*temp = strtok_r(buf, ";", &semi_save);
				i = 0;
				j = 0;
				tok(*temp, com);
				exec(com,fileNAME);
				puts("");

				while( (*temp = strtok_r(NULL, ";", &semi_save)) != NULL ){
					i = 0 ;
					j = 0 ;
					tok(*temp, com);
					exec(com,fileNAME);
					puts("");
				}//end while
			}else{
				tok(buf, com);
				if(exec(com, &fileNAME) == 10)
					return 0;
			}//end if(';')
		}//end if ("\0")

	}//while(1)の終了
	
	return 0;

}//mainの終了

void tok(char *buf, char **com){
	int i = 1;
	com[0] = strtok(buf," ");
	while( (com[i] = strtok(NULL, " ")) != NULL){
		++i;
	}// end while
}//end tok function

void initialize(char *buf){
	int i = 0;
	for( i = 0 ; i < BUFSIZE ; i++ )
			buf[i] = 0;
}//end initilize function
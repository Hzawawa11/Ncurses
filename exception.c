
#include "shell_int.h"

void exception(char *buf){
/*variable*/
	char input[BUFSIZE];
	int i = 0 ;
	int buf_num = 0 ;


	i = 0;//initialize
	buf_num = 0;

/*nothing text*/
	if(fgets(input,BUFSIZE,stdin) != NULL){
		/*kaigyou moji made*/
		while(input[i] != '\n'){
			/*  if char is ' ' and first character, ignore
			 *  not first character, store ' ' to com[] */
			if(input[i] == ' '){
				if( buf_num != 0 ){
					while(input[i] == ' '){
						i++ ;
					}
					buf[buf_num] = ' ' ;//
					buf_num++ ;
					i--;
				}//end if(buf_num != 0)
			}	
		else{
			buf[buf_num] = input[i];
			buf_num++;
		}//end if(input[i] == ' ')		
		i++;
	}
	/*initialize*/
	for(i = 0 ; i < BUFSIZE ; i++)
		input[i] = 0;

	}
}


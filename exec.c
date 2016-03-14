/* ******************************** 
 *	date : 2014/08/15
 *	name : Hanzawa JUNICHI
 *	abstruct : my shell
 * ****************************** */

/*preprocessor*/
#include "shell_int.h"
int exec(char **com, char **fileNAME){
	pid_t ret;
	pid_t tmp;

	/*今回つくったファイルの中身みるためのコマンド*/
	if(strcmp(com[0],"file") == 0){
		// printf("%s\n", com[1]);
		strcpy(*fileNAME, com[1]);
		// fileNAME = com[1];
		// printf("FILE is %s\n", fileNAME);
		return 10;	
	}
	/*
	if(strcmp(com[0],"run") == 0){
		printf("####... run.\n");
		return 10;	
	}
	*/
	/*外部コマンド実行(exterior comand)*/
	/*cdコマンド実行(cd comand)*/
	if(strcmp(com[0],"cd") == 0){
		chdir(com[1]);
		return 1;	
	}
		
	/*exitコマンド実行(exit comand)*/
	if(strcmp(com[0],"exit") == 0)
		_exit(0);

	ret = fork();				//子プロセス作成

	/*プロセス作成失敗(failed make process)*/
	if (ret < 0){
		perror("fork");
		exit(1);
	}



	/*子プロセスの場合(child process)*/
	if (ret == 0){
		tmp = execvp(com[0],com);//内部コマンドの実行

		/*コマンド実行失敗(failed exep comand)*/
		if(tmp < 0){
			perror("execvp");
			exit(1);
		}
			
		/*[cd]の例外処理(cd exception)*/
		else if(strcmp(com[0],"cd") == 0){
			perror("cd");
			exit(1);
		}

		/*[exit]の例外処理(exit exception)*/
		else if(strcmp(com[0],"exit") == 0){
			perror("exit");
			exit(1);
		}
	} 


	/*親プロセスの処理(parent process)*/
	else {
		if (wait(NULL) < 0){
			perror("wait");
			exit(1);
		}//end if wait処理	
	}//end parent

	return 0;
}//end exec


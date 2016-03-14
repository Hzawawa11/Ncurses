#include <stdio.h>

int main(void){

	int x;
	int i;
	for(i = 0 ; i< 10 ; i++){
		if(	i == 7 ){
//			break;
		}
	}
	if(){

	}
	else{

	}

	if(){
		for(){
		}
		if(){
		}
		for(){
			if(){
			}
		}
		if(){
		}
		// while(){
		// 	if(){
		// 		continue;
		// 	}
		// 	switch(){
		// 		case'0':
		// 			break;
		// 		case'1':
		// 			break;
		// 		default:
		// 			break;
		// 	}
		// }

	
	return 0;
}

/*
./analyzer test.c
0000  	0000	main	0		73
0001  	0002	if		8		10
0002  	0001	for		7		11
0003  	0001	if		12	14
0004  	0001	else	15	17
0005  	0002	for		20	21
0006  	0002	if		22	23
0007  	0003	if		25	26
0008  	0002	for		24	27
0009  	0002	if		28	29
0010  	0001	if		19	46
*/

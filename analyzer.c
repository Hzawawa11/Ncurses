/* *Error Report***************************************************************
*  switchは1重しか対応していない
* *****************************************************************************/ 

/* Header File ******************************************************************/
#include "shell_int.h"

/* Prototype ***********************************************************/
void prod_structure(int ,char *);       // 
void Nest(int);                         //
void check_braces(int);                 //
void check_flows(int, char *);          //
int pop(void);                          //
int linepop(void);                      //
void push(int);                         //
void linepush(int);                     //
void NextChar(void);                    //
void skip_word(void);                   //
void skip_string(void);                 //
void skip_const_char(void);             //
void skip_comment(void);                //
int FILE_LINE(char *fname);

/*Structure***********************************************************************/
 typedef struct _result{
   int number;
   int nest ;
   char name[CHARACTER];
   struct _result *next;
   int start_line;
   int end_line;
 } result;

/* Global Variable ***********************************************************/
FILE *Fp;                               //File pointer
int Ch, ChNext;                         //In order to store character
int count = 0;
int Tab = 0;                            //Nesting Tab
int TabMAX = 0;
int Stack[Stack_SIZE];                    //Stack Memory
int lineStack[Stack_SIZE];
int Sp;                                 //Stuck Point
int lineSp;                                 //Stuck Point
int Range = 0;                              //Scope and Block
// 0 is none scope. 1 is if scope. 2 is for scope.
int branch = 0;       //default brach flow chart.
int loop = 0;         //default loop flow chart.
int case_default = 0;
int conserve_count = 1;
int Line = 0;         //store number of the line 
result *p, *start;   //pointer of structure 

/* Testing function******************************************/
/*
int Check(char *iden, int Range){

  if( Ch == iden[0] && ChNext == iden[1]){    //if statement
    NextChar();NextChar();
    // printf("%c\n", Ch);
    if(!isalpha(Ch) && Ch != '_'){
      // printf("aaaa\n");
      return Range;
    }
  }
  return 0;
  
  int i = 0;
  
  // printf("%lu\n",strlen(iden));

  for( i = 0 ; i < strlen(iden) ; i+=2 ){
    if( Ch == iden[0+i] && ChNext == iden[1+i] ){
      NextChar();NextChar();
      // printf("%s : Success!!\n", iden);
    }else{
      // NextChar();NextChar();m
      printf("check1 : %c %c\n", Ch, ChNext);
      skip_word();
      return 0;
    }
  }
  
  if( !isalpha(Ch) && Ch != '_' ){
    return Range;
  }else{
    skip_word();
    printf("check2 : %c %c\n", Ch, ChNext);
    return 0;
  }
  printf("check3 : %c %c\n", Ch, ChNext);
  return 0;
}
*/

/* Main Data fucntion ******************************************************************/
void Data(char *fname){   //argc is  , fname is file name
  //Check that can open file
  if((Fp = fopen(fname, "r")) == NULL){
    fprintf(stderr, "Can't open %s' (No such file or directory)\n", fname);
    exit(EXIT_FAILURE);
  }
  
  /* Initialization ************************************/
  count = 0;
  Line++;
  Ch = fgetc(Fp);                       // Current character
  ChNext = fgetc(Fp);                   // Next character
  push(Range);                          // Initialize none scope
  linepush(Line);                       // Initialize none scope

  /* save first pointer */
  p = malloc(sizeof(result)); //
  strcpy(p->name, "main");    //store a string "main"
  p->end_line = FILE_LINE(fname);  //
  start = p;                  //

  //Reading character
  while(Ch != EOF){
    if(isalpha(Ch) || (Ch == '_')){     //if Alphabet and underscore
      Nest(Tab);


      //if statement
      
      if( Ch == 'i' && ChNext == 'f'){    //if statement
        Range = 1;                        //scopechange
        NextChar();NextChar();
        continue;
      }
      

      /*
      printf("CH : %c\n", Ch);
      printf("Chnext : %c\n", ChNext);
      

      if(Check("if", 1) == 1){
        Range = 1;
        continue;
      }

      // printf("not!!\n");
      // if(Check("else", 6) == 6){
        // Range = 6;
        // continue;
      // }
      */

      //Else statement
      if(Ch == 'e' && ChNext == 'l' ){              
        // printf("skip\n");
        NextChar();NextChar();
        if(Ch == 's' && ChNext == 'e'){
          NextChar();NextChar();
          if( !isalpha(Ch) && Ch != '_' ){
            Range = 6;
            //check_flows(Range, "else");
            // NextChar();
            continue;
          }else{
            skip_word();
            count++;
            continue;
          }
        }else{
          // NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }

      printf("main : %c %c\n", Ch, ChNext);
      //For statement
      if(Ch == 'f' && ChNext == 'o' ){ 

        // printf("aaaaasdadadada\n");
        NextChar();NextChar();
        if(Ch == 'r' && !isalpha(ChNext) && ChNext != '_' ){
          // printf("FOR : ");
          Range = 2;                      //scope change
          NextChar();NextChar();
          continue;
        }else{
          skip_word();
          count++;
          continue;
        }
      }

      //While statement
      if(Ch == 'w' && ChNext == 'h' ){              
        NextChar();NextChar();
        if(Ch == 'i' && ChNext == 'l'){
          NextChar();NextChar();
          if(Ch == 'e' && !isalpha(ChNext) && ChNext != '_' )  {
            Range = 3;                      //scope change
            NextChar();NextChar();
            continue;
          }else{
            skip_word();
            count++;
            continue;
          }
        }else{
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }

      //Switch statement
      if(Ch == 's' && ChNext == 'w' ){             
        NextChar();NextChar();
        if(Ch == 'i' && ChNext == 't'){
          NextChar();NextChar();
          if(Ch == 'c' && ChNext == 'h'){
            NextChar(); 
            if(!isalpha(ChNext) && ChNext != '_' )  {
              NextChar();NextChar();
              Range = 4;                         //scope change
              continue;
            }else{
              NextChar();
              skip_word();
              count++;
              continue;
            }
          }else{
            NextChar();NextChar();
            skip_word();
            count++;
            continue;
          }
        }else{
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }

      //Case statement
      if(Ch == 'c' && ChNext == 'a' ){              
        NextChar();NextChar();
        if(Ch == 's' && ChNext == 'e'){
          NextChar();NextChar();
          if( !isalpha(Ch) && Ch != '_' )  {
            case_default++;
            Range = 5;
            check_flows(Range, "case");
            NextChar();
            continue;
          }else{
            skip_word();
            count++;
            continue;
          }
        }else{
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }

      //Default Statement
      if(Ch == 'd' && ChNext == 'e' ){              
        NextChar();NextChar();
        if(Ch == 'f' && ChNext == 'a'){
          NextChar();NextChar();
          if(Ch == 'u' && ChNext == 'l'){
            NextChar();NextChar();
            if(Ch == 't' && !isalpha(ChNext) && ChNext != '_' )  {
              Range = 5;
              check_flows(Range, "default");
              NextChar();
              continue;
            }else{
              // printf("defaul%c", Ch);
              NextChar();
              skip_word();
              count++;
              continue;
            }
          }else{
            //printf("defa%c%c", Ch, ChNext);
            NextChar();NextChar();
            skip_word();
            count++;
            continue;
          }
        }else{
          //printf("de%c%c", Ch, ChNext);
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }
      
      //Return statement
      if(Ch == 'r' && ChNext == 'e' ){             
        NextChar();NextChar();
        if(Ch == 't' && ChNext == 'u'){
          NextChar();NextChar();
          if(Ch == 'r' && ChNext == 'n'){
            NextChar(); 
            if(!isalpha(ChNext) && ChNext != '_' )  {
              NextChar();NextChar();
              Range = 4;                      //scope change
              continue;
            }else{
              NextChar();
              skip_word();
              count++;
              continue;
            }

          }else{
            NextChar();NextChar();
            skip_word();
            count++;
            continue;
          }

        }else{
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }

      //Break
      if(Ch == 'b' && ChNext == 'r' ){              
        NextChar();
        NextChar();
        if(Ch == 'e' && ChNext == 'a'){
          NextChar();
          NextChar();
          if(Ch == 'k' && !isalpha(ChNext) && ChNext != '_' )  {
            Tab--;
            loop = 0;
            NextChar();NextChar();
            continue;
          }else{
            skip_word();
            count++;
            continue;
          }
        }else{
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }

      //continue
      if(Ch == 'c' && ChNext == 'o'){              
        NextChar();NextChar();
        if(Ch == 'n' && ChNext == 't'){
          NextChar();NextChar();
          if(Ch == 'i' && ChNext == 'n'){
            NextChar();NextChar();
            if(Ch == 'u' && ChNext == 'e'){
              NextChar();
              if(!isalpha(ChNext) && ChNext != '_' ){
                NextChar();
                continue;
              }else{
                skip_word();
                count++;
                continue;
              }
            }else{
              NextChar();
              skip_word();
              count++;
              continue;
            }
          }else{
            NextChar();NextChar();
            skip_word();
            count++;
            continue;
          }
        }else{
          NextChar();NextChar();
          skip_word();
          count++;
          continue;
        }
      }
      count++;                          //Counting Word
      NextChar();
      skip_word();                      //When character in a row  
    }// end if that isalpha(Ch) || (Ch == '_'))

    else if (Ch == '{')  {
      push(Range);
      // printf("Range : %d\n",Range);
      // printf("Line:%d\n", Line);
      linepush(Line);
      Nest(Tab);
      // if(Range != 4)
      // check_flows(Range, "");    //ほかは直接入力なので影響は受けない
      // printf("start ({)");              //Output
      if(Tab > TabMAX)
        TabMAX = Tab;
      Tab++;                            //increase number of Nesting
      NextChar();                   //next reading charater
    }
    else if (Ch == '}') {
      Tab--;                          //decrease number of Nesting
      Nest(Tab);
      // check_braces(pop());  
      check_flows(pop(), "");    
      // printf("linepop(): %d | Line: %d\n", linepop(), Line);                
      // printf("end (})");
      NextChar();                   //next reading charater
    } 
    else if (Ch == ':') {
      if(Tab > TabMAX)
        TabMAX = Tab;
      Tab++;                          //decrease number of Nesting
      Nest(Tab);
      //printf(" : ");
      NextChar();                   //next reading charater
    } 
    else if (Ch == '\n') {
      Line++;
      // printf(" [ %dbranches : ", branch);
      // printf("%dloops ]", loop);
      //puts("");                         //kaigyou code
      NextChar();                   //next reading charater
    }
    else if (Ch == '\"')                //double col
      skip_string();

    else if (Ch == '\'')                //single col
      skip_const_char();

    else if (Ch == '/' && ChNext == '*')
      skip_comment();
    
    else if(Ch == '/' && ChNext == '/'){            //
      NextChar();
      for(;;){
        NextChar();
        if(Ch == '\n')
          break;
      }
    }
    else
      NextChar();
  }//end while

  fclose(Fp);               //close File
  
  // テーブルで表示
   /* Use to debug*/
    /*
   p = start;  // set first pointer
   while (p != NULL) {
     printf("%04d  \t%04d\t%s\t%d\t%d\n", p->number, p->nest ,p->name, p->start_line, p->end_line);
     p = p->next;
   }
   */
  p = start;  // ポインタを先頭に //
}//end main function


/*Other function********************************************************************/
void prod_structure(int num, char *character){  
  p->next = malloc(sizeof(result));
  p = p->next;

  /* numberはカウンタ変数から生成 */
  p->end_line = Line;
  p->start_line = linepop();
  p->number = conserve_count;
  conserve_count++;
  p->nest = num;

  /* 2項目を代入 */
  strcpy(p->name, character);
  p->next = NULL;
}

void check_flows(int flow, char *kind){
  switch(flow){
    case 0:
      break;
    case 1: //if 
      branch++;
      prod_structure(Tab, "if");
      break;
    case 2: // for
      loop++;
      prod_structure(Tab, "for");
      break;
    case 3: //while
      loop++;
      prod_structure(Tab, "while");
      break;
    case 4: // switch
      branch++;
      prod_structure(Tab, "switch");
      break;
    case 5:
      prod_structure(Tab, kind);
      branch++;
      break;
    case 6: // else
      branch++;
      prod_structure(Tab, "else");
      break; 

    default:
      break;
  }
}

//Control stack 
int pop(){
  int pop_num;
  pop_num = Stack[Sp];
  Sp--; 
  return pop_num;
}
void push(int data){
  Sp++;
  Stack[Sp] = data;
}

int linepop(){
  int pop_num;
  pop_num = lineStack[lineSp];
  lineSp--; 
  return pop_num;
}

void linepush(int data){
  lineSp++;
  lineStack[lineSp] = data;
}

//Scan Next and Current Character
void NextChar(){
  Ch = ChNext;
  ChNext = fgetc(Fp); 
}

void Nest(int num){
  for( ; num > 0 ; num--){
    ;
  }
}//end function

//involve next character When scan word equal to sentence
void skip_word(void){
  while (isalnum(Ch) || (Ch == '_'))
    NextChar();
}//end function


void skip_string(void){
  NextChar();
  while(Ch != '\"'){
    if (Ch == '\\') 
      NextChar();
    NextChar();
  }
  NextChar();
}//end function

void skip_const_char(void){
  NextChar();
  while (Ch != '\''){
    if(Ch == '\\')  
      NextChar();
    NextChar();
  }
  NextChar();
}//end function

void skip_comment(void){
  NextChar();
  NextChar();
  for(;;){
    if(Ch == '*'){
      NextChar();
      if(Ch == '/')   
        break;    
    }
    NextChar();
  }
  NextChar();
}//end function

int FILE_LINE(char *fname){
  int counter = 0;
  FILE *fp;

  fp = fopen(fname,"r");
  int key;

  if(fp) {
    while((key = getc(fp)) != EOF)
      if(key == '\n') counter++;
  }
  fclose(fp);
  return counter;
}
/*****************************************************************************/

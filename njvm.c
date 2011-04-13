#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "njvm.h"

#define HALT 0
#define PUSHC 1 /*pushc <n>*/
#define ADD 2 
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8

#define ASF 9 /*asf <n>*/
#define RSF 10
#define PUSHL 11 /*pushl <n>*/
#define POPL 12 /*popl <n>*/

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))

#define stackSize 1024

const char version[] = "0.1";
/*const int stackSize = 1024;*/
int stack[stackSize];
int stackPointer=0;
int framePointer=0;
int programCounter=0; /* zählt die Zeilen bei der Ausgabe */
unsigned int code1[] = {(PUSHC << 24) | IMMEDIATE(3),
                        (PUSHC << 24) | IMMEDIATE(4),
                        (ADD << 24),
                        (PUSHC << 24) | IMMEDIATE(10),
                        (PUSHC << 24) | IMMEDIATE(6),
                        (SUB << 24),(MUL << 24),
                        (WRINT << 24),(HALT << 24)};

unsigned int code2[] = {(PUSHC << 24) | IMMEDIATE(-2),
	                (RDINT << 24),
	                (MUL << 24),
	                (PUSHC << 24) | IMMEDIATE(3),
	                (ADD << 24),
                        (WRINT << 24), (HALT << 24)};


int main(int argc, char *argv[]){
  if(argc >= 2){
    int i;
    for(i=1;i<argc;i++){
      /* Hilfe ausgeben */
      if(strcmp(argv[i],"--help")==0){
        printHelp();
      }else if(strcmp(argv[i],"--version")==0){
        /* Versionsinformationen ausgeben */
        printf("Ninja Virtual Machine version %s (compiled %s, %s)\n",version,__DATE__,__TIME__);
      }else if(strcmp(argv[i],"--program")==0){
        if(strcmp(argv[i+1],"1")==0){
	  program(code1,sizeof(code1)/sizeof(code1[0]));
          break;
        }else if(strcmp(argv[i+1],"2")==0){
	  program(code2,sizeof(code2)/sizeof(code2[0]));
          break;
        }
      }else{
        /* Unbekannter Befehl */
	printf("unknown command line argument '%s', try './njvm --help' \n", argv[i]);
      }
    }
  }else{
    printf("No Arguments, try --help.\n");
  }

   return 0;
}


/* Ausgabe von Option */
void printHelp(void){
  printf("./njvm [option] [option] ...\n");
  printf("--program 1\n");
  printf("--program 2\n");
  printf("--help     print this page\n");
  printf("--version  print version of programm\n");
}

/* Gibt das Programm aus */
void printProgram(unsigned int *code, int size){
  int zeile, i;
  
  for(i=0;i<size;i++){
     
    zeile = (code[i]&0xFF000000)>>24;
  
    if(zeile==HALT){
      printf("%03d: halt\n",programCounter);
    }else if(zeile==PUSHC){
      printf("%03d: pushc %d\n", programCounter, (SIGN_EXTEND(code[i]&0x00FFFFFF)));
    }else if(zeile==ADD){
      printf("%03d: add\n",programCounter);
    }else if(zeile==SUB){
      printf("%03d: sub\n",programCounter);
    }else if(zeile==MUL){
      printf("%03d: mul\n",programCounter);
    }else if(zeile==DIV){
      printf("%03d: div\n",programCounter);
    }else if(zeile==MOD){
      printf("%03d: mod\n",programCounter);
    }else if(zeile==RDINT){
      printf("%03d: rdint\n",programCounter);
    }else if(zeile==WRINT){
      printf("%03d: wrint\n",programCounter);
    }else if(zeile==ASF){
      printf("%03d: asf\n",programCounter);
    }else if(zeile==RSF){
      printf("%03d: rsf\n",programCounter);
    }else if(zeile==PUSHL){
      printf("%03d: pushl\n",programCounter);
    }else if(zeile==POPL){
      printf("%03d: popl\n",programCounter);
    }

    programCounter++;
    
  }
  programCounter = 0;
}

/*code1 und code2 ausfuehren*/
void program(unsigned int *code,int size){
  int instruction, i, n1, n2, eingeleseneZahl;

  printf("Ninja Virtual Machine started\n");

  printProgram(code, size);

  /*geht jede Instruktion der Instruktionstabelle durch*/
  for(i=0;i<size;i++){
    instruction=(code[i]&0xFF000000)>>24;

    if(instruction==HALT){
      break;
    }else if(instruction==PUSHC){ /*schreiben in stack*/
      push(code[i]);
    }else if(instruction==ADD){
      n1=pop();
      n2=pop();
      push(n2+n1);
    }else if(instruction==SUB){
      n1=pop();
      n2=pop();
      push(n2-n1);
    }else if(instruction==MUL){
      n1=pop();
      n2=pop();
      push(n2*n1);
    }else if(instruction==DIV){
      n1=pop();
      n2=pop();
      
      if(n2!=0){
        push(n2/n1);
      }
    }else if(instruction==MOD){
      n1=pop();
      n2=pop();
      push(n1%n2);
    }else if(instruction==RDINT){
      scanf("%d", &eingeleseneZahl);
      push(eingeleseneZahl);
    }else if(instruction==WRINT){
      printf("%d\n",SIGN_EXTEND(stack[stackPointer-1]&0x00FFFFFF));
    }else if(instruction==ASF){
      
    }else if(instruction==RSF){
      
    }else if(instruction==PUSHL){
      
    }else if(instruction==POPL){
      
    }
  }

  stackPointer=0;
  printf("Ninja Virtual Machine stopped\n");
}

void push(int num){
  stack[stackPointer]=(num&0x00FFFFFF);
  stackPointer++;
  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  if(stackPointer > stackSize){
    printf("Stackposition out of Range. Program will be stopped.\n");
    exit(-99);
  }
}

int pop(void){
  stackPointer--;
  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  if(stackPointer < 0){
    printf("Stackposition out of Range. Program will be stopped.\n");
    exit(-99);
  }
  return stack[stackPointer];
}

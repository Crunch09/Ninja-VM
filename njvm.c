#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "njvm.h"

#define HALT 0
#define PUSHC 1
#define ADD 2 
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))

const char version[] = "0.1";
int stack[1024];
int stackPosition=0;
int programCounter=0;
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
        	program(code1,sizeof(code1)/sizeof(int));
          break;
        }
        else if(strcmp(argv[i+1],"2")==0){
        	program(code2,sizeof(code2)/sizeof(int));
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


/*code1 und code2 ausfuehren*/
void program(unsigned int *code,int size){
  int count, instruction, i, n1, n2, eingeleseneZahl;
  count=0;

  printf("Ninja Virtual Machine started\n");

  /*geht jede Instruktion der Instruktionstabelle durch*/
  for(i=0;i<size;i++){
    instruction=(code[i]&0xFF000000)>>24;

    if(instruction==HALT){
      printf("%03d: halt\n",programCounter);
      break;
    }else if(instruction==PUSHC){ /*schreiben in stack*/
      printf("%03d: pushc  %d\n",programCounter,(SIGN_EXTEND(code[i]&0x00FFFFFF)));
      push(code[i]);
      /*stack[stackPosition]=(code[i]&0x00FFFFFF);
	    printf("\n test %04d\n", stack[stackPosition]);
      stackPosition++;*/
    }else if(instruction==ADD){
      printf("%03d: add\n",programCounter);
      n1=pop();
      n2=pop();
      push(n2+n1);
      /*stack[stackPosition-1]&0x00FFFFFF; Zahl oberste im Stack herausgenommen
      stack[stackPosition-2]&0x00FFFFFF; Zahl, zweite von oben im Stack herausgenommen
      stack[stackPosition-2]=n1+n2; rechenoperation
      stackPosition--; stackposition veraendern*/
    }else if(instruction==SUB){
      printf("%03d: sub\n",programCounter);
      n1=pop();
      n2=pop();
      push(n2-n1);
      /*stack[stackPosition-2]&0x00FFFFFF;
      stack[stackPosition-1]&0x00FFFFFF;
      stack[stackPosition-2]=n1-n2;
      stackPosition--;*/
    }else if(instruction==MUL){
      printf("%03d: mul\n",programCounter);
      n1=pop();
      n2=pop();
      push(n2*n1);
      /*n1=stack[stackPosition-1]&0x00FFFFFF;
      n2=stack[stackPosition-2]&0x00FFFFFF;
      stack[stackPosition-2]=n1*n2;
      stackPosition--;*/
    }else if(instruction==DIV){
      printf("%03d: div\n",programCounter);
      n1=pop();
      n2=pop();
      /*n1=stack[stackPosition-1]&0x00FFFFFF;
      n2=stack[stackPosition-2]&0x00FFFFFF;*/
      /*teilen durch null abfangen*/
      if(n2!=0){
        /*stack[stackPosition-2]=n1/n2;*/
        push(n2/n1);
      }
      /*stackPosition--;*/
    }else if(instruction==MOD){
      printf("%03d: mod\n",programCounter);
      n1=pop();
      n2=pop();
      push(n1%n2);
      /*n1=stack[stackPosition-1]&0x00FFFFFF;
      n2=stack[stackPosition-2]&0x00FFFFFF;
      stack[stackPosition-2]=n1%n2;
      stackPosition--;*/
    }else if(instruction==RDINT){
	    /* liest Zahl auf der konsole ein */
	    printf("%03d: rdint ",programCounter);
	    scanf("%d", &eingeleseneZahl);
      push(eingeleseneZahl);
	    /*stack[stackPosition] = eingeleseneZahl;
	    stackPosition++;*/
    }else if(instruction==WRINT){
      printf("%03d: wrint %d\n",programCounter,SIGN_EXTEND(stack[stackPosition-1]&0x00FFFFFF));
    }
    /* count für Zeilenausgabe erhöhen */
	  programCounter++;
  }

  stackPosition=0;
  programCounter=0;
	printf("Ninja Virtual Machine stopped\n");
}

void push(int num){
  stack[stackPosition]=(num&0x00FFFFFF);
  stackPosition++;
}

int pop(void){
  stackPosition--;
  return stack[stackPosition];
}

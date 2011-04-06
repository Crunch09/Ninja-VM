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
        if(strcmp(argv[i+1],"1")==0){}
          /*program1();*/
          
        else if(strcmp(argv[i+1],"2")==0){}
          /*program2();*/
	    }else{
        /* Unbekannter Befehl */
			  printf("unknown command line argument '%s', try './njvm --help' \n", argv[i]);
		  }
    }
	}else{
		/* Keine Parameter angegeben 
		printf("Ninja Virtual Machine started\n");
		printf("Ninja Virtual Machine stopped\n");*/
	}
	
	program();
	
	
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
void program(void){
  int count, instruction, i, n1, n2, eingeleseneZahl;
  count=0;

  printf("Ninja Virtual Machine started\n");

  /*geht jede Instruktion der Instruktionstabelle durch*/
  for(i=0;i<sizeof(code2)/sizeof(int);i++){
    instruction=(code2[i]&0xFF000000)>>24;

    if(instruction==HALT){
      printf("%03d: halt\n",count);
      break;
    }else if(instruction==PUSHC){ /*schreiben in stack*/
      printf("%03d: pushc  %d\n",count,(code2[i]&0x00FFFFFF));
      stack[stackPosition]=(code2[i]&0x00FFFFFF);
	  /* printf("\n test %04d\n", stack[stackPosition]); */
      stackPosition++;
    }else if(instruction==ADD){
      printf("%03d: add\n",count);
      n1=stack[stackPosition-1]&0x00FFFFFF; /*Zahl oberste im Stack herausgenommen*/
      n2=stack[stackPosition-2]&0x00FFFFFF; /*Zahl, zweite von oben im Stack herausgenommen*/
      stack[stackPosition-2]=n1+n2; /*rechenoperation*/
      stackPosition--; /*stackposition veraendern*/
    }else if(instruction==SUB){
      printf("%03d: sub\n",count);
      n1=stack[stackPosition-2]&0x00FFFFFF;
      n2=stack[stackPosition-1]&0x00FFFFFF;
      stack[stackPosition-2]=n1-n2;
      stackPosition--;
    }else if(instruction==MUL){
      printf("%03d: mul\n",count);
      n1=stack[stackPosition-1]&0x00FFFFFF;
      n2=stack[stackPosition-2]&0x00FFFFFF;
      stack[stackPosition-2]=n1*n2;
      stackPosition--;
    }else if(instruction==DIV){
      printf("%03d: div\n",count);
      n1=stack[stackPosition-1]&0x00FFFFFF;
      n2=stack[stackPosition-2]&0x00FFFFFF;
      /*teilen durch null abfangen*/
      if(n2!=0){
        stack[stackPosition-2]=n1/n2;
      }
      stackPosition--;
    }else if(instruction==MOD){
      printf("%03d: mod\n",count);
      n1=stack[stackPosition-1]&0x00FFFFFF;
      n2=stack[stackPosition-2]&0x00FFFFFF;
      stack[stackPosition-2]=n1%n2;
      stackPosition--;
    }else if(instruction==RDINT){
	  scanf("%d", &eingeleseneZahl);
	  printf("%03d: rdint %d\n",count, eingeleseneZahl);
	 stack[stackPosition] = eingeleseneZahl;
	 stackPosition++;
    }else if(instruction==WRINT){
      printf("%03d: wrint %d\n",count,SIGN_EXTEND(stack[stackPosition-1]&0x00FFFFFF));
    }
    /* count für Zeilenausgabe erhöhen */
	count++;
  }

  stackPosition=0;
	printf("Ninja Virtual Machine stopped\n");
}

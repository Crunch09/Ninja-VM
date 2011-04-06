#include <stdio.h>
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

const char version[] = "0.1";
unsigned int code1[] = {(PUSHC << 24) | IMMEDIATE(3),
                        (PUSHC << 24) | IMMEDIATE(4),
                        (ADD << 24),
                        (PUSHC << 24) | IMMEDIATE(10),
                        (PUSHC << 24) | IMMEDIATE(6),
                        (SUB << 24),(MUL << 24),
                        (WRINT << 24),(HALT << 24)};



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
		/* Keine Parameter angegeben */
		printf("Ninja Virtual Machine started\n");
		printf("Ninja Virtual Machine stopped\n");
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

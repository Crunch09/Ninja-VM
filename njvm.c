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

#define stackSize 1024

const char version[] = "0.2";
int stack[stackSize];
int stackPosition=0;
int programCounter=0; /* zählt die Zeilen bei der Ausgabe */
FILE *codeFile;
int fileSize, numberOfCommands;
unsigned int *programPointer;


int main(int argc, char *argv[]){
  if(argc >= 2){
    int i, fileClosed;
    for(i=1;i<argc;i++){
		  /* Hilfe ausgeben */
  	  if(strcmp(argv[i],"--help")==0){
	      printHelp();
      }else if(strcmp(argv[i],"--version")==0){
	      /* Versionsinformationen ausgeben */
	      printf("Ninja Virtual Machine version %s (compiled %s, %s)\n",version,__DATE__,__TIME__);
      
	  }else{
		codeFile = fopen(argv[i], "r");
		if(codeFile == NULL || (strstr(argv[i], ".bin") == NULL )){
			printf("Couldn't open %s . You need to use a .bin file. Try again. \n", argv[i]);
		}else{
			printf("Opened %s successful. \n", argv[i]);
			/* Zeiger ans Ende setzen, um Größe zu ermitteln */
			fseek(codeFile, 0, SEEK_END);
			fileSize = ftell(codeFile);
			/* Zeiger wieder an den Anfang setzen */
			fseek(codeFile, 0, SEEK_SET);
			programPointer = malloc(fileSize);
			if( programPointer == NULL){
				printf("No RAM available. \n");
				exit(-99);
			}
			printf("Size: %d Bytes \n", fileSize);
			numberOfCommands = fread(programPointer, 4, fileSize/4, codeFile);
			if(numberOfCommands < fileSize/sizeof(programPointer[0])){
				printf("There occured an error reading the file.  \n");
				exit(-99);
			}
			program(programPointer, fileSize/sizeof(programPointer[0]));
			fileClosed = fclose(codeFile);
			if(fileClosed != 0){
				printf("Warning ! Couldn't close the file. \n");
			}
		}
       
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
printf("./njvm path/to/filename.bin executes this code\n");
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
       printf("%03d: rdint \n",programCounter);
     }else if(zeile==WRINT){
       printf("%03d: wrint \n",programCounter);
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
      /*stackPosition--;*/
    }else if(instruction==MOD){
      n1=pop();
      n2=pop();
      push(n1%n2);

    }else if(instruction==RDINT){
	    /* liest Zahl auf der konsole ein */
	  scanf("%d", &eingeleseneZahl);
      push(eingeleseneZahl);
    }else if(instruction==WRINT){
      printf("%d\n",SIGN_EXTEND(stack[stackPosition-1]&0x00FFFFFF));
    }

  }

  stackPosition=0;
  /*programCounter=0;*/
  printf("Ninja Virtual Machine stopped\n");
}

void push(int num){
  stack[stackPosition]=(num&0x00FFFFFF);
  stackPosition++;
  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  if(stackPosition > stackSize){
    printf("Stackposition out of Range. Program will be stopped.\n");
    exit(-99);
  }
}

int pop(void){
  stackPosition--;
  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  if(stackPosition < 0){
    printf("Stackposition out of Range. Program will be stopped.\n");
    exit(-99);
  }
  return stack[stackPosition];
}

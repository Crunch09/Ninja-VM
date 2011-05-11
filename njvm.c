#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "njvm.h"

/* begin instructions */
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
#define PUSHL 11 /*pushl <n>, wert von frame in stack*/
#define POPL 12 /*popl <n>, wert von stack in frame*/

#define EQ 13  /* == */
#define NE 14  /* != */
#define LT 15  /* <  */
#define LE 16  /* <= */
#define GT 17  /* >  */
#define GE 18  /* >= */

#define JMP 19 /* jmp <target> */
#define BRF 20 /* brf <target> */
#define BRT 21 /* brt <target> */

#define CALL 22 /* call <target> */
#define RET 23
#define DROP 24 /* drop  <n> */
#define PUSHR 25
#define POPR 26

#define DUP 27
/* end instructions */

#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))

#define stackSize 1024

const char version[] = "0.4.1";
int stack[stackSize];
int returnRegister;
int stackPointer=0;
int framePointer=0;
int programCounter=0; /* zählt die Zeilen bei der Ausgabe */
FILE *codeFile;
int fileSize, fileClosed, numberOfCommands;
unsigned int *programPointer;


/* main */
int main(int argc, char *argv[]){
  if(argc >= 2){
    int i;/*, fileClosed;*/
    for(i=1;i<argc;i++){
      /* Hilfe ausgeben */
      if(strcmp(argv[i],"--help")==0){
        printHelp();
      }else if(strcmp(argv[i],"--version")==0){
        /* Versionsinformationen ausgeben */
        printf("Ninja Virtual Machine version %s (compiled %s, %s)\n",version,__DATE__,__TIME__);
      }else if(strcmp(argv[i],"--debug")==0){
	openFile(i+1,argc,argv);
	debug();
	closeFile();
	break;
      }else{
	openFile(i,argc,argv);
	printf("Ninja Virtual Machine started\n");
	for(programCounter=0;programCounter<(fileSize/sizeof(programPointer[0]));programCounter++)
	  program(programPointer);
	closeFile();
	printf("Ninja Virtual Machine stopped\n");
      }
    }    
  }else{
    printf("No Arguments, try --help.\n");
  }

   return    printf("Ninja Virtual Machone stopped\n");
    exit(0); 0;
}


/* open a bin-file */
void openFile(int i,int argc, char *argv[]){
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
    printf("Size: %d Bytes \n\n", fileSize);
    numberOfCommands = fread(programPointer, 4, fileSize/4, codeFile);
    if(numberOfCommands < fileSize/sizeof(programPointer[0])){
      printf("There occured an error reading the file.  \n");
      exit(-99);
    }
  }    
}
/* close a file */
void closeFile(void){
  fileClosed = fclose(codeFile);
  if(fileClosed != 0){
    printf("Warning ! Couldn't close the file. \n");
  }
}


/* Ausgabe von Option */
void printHelp(void){
  printf("./njvm [option] [option] ...\n");
  printf("./njvm path/to/filename.bin    executes this code\n");
  printf("--help                         print this page\n");
  printf("--version                      print version of programm\n");
  printf("--debug path/to/filename.bin   debug mode for program\n\n");
}


/* debuger */
void debug(void){
  int numberInstructions,tempInstruction,j;
  char inputString[5];

  numberInstructions=fileSize/sizeof(programPointer[0]);
  programCounter=0;

  printf("Ninja Virtual Machine started\n");
  printProgram(programPointer);

  while(programCounter<numberInstructions){
    printf("DEBUG: is (inspect stack), l (list), s (step), r (run), q (quit)?: ");
    scanf("%s",inputString);

    if(strcmp(inputString,"is")==0){
      for(j=stackPointer;j>-1;j--){ /* stack von oben nach unten durchgehen */
	      if(j==stackPointer && j==framePointer)
	        printf("sp, fp ---> %04d: xxxx\n",j);
	      else if(j==stackPointer)
	        printf("sp -------> %04d: xxxx\n",j);
	      else if(j==framePointer)
	        printf("fp -------> %04d: %4d\n",j,stack[j]);
	      else
	        printf("            %04d: %4d\n",j,stack[j]);
      }
      printf("--- bottom of stack ---\n");
      printProgram(programPointer);
    }else if(strcmp(inputString,"l")==0){
      tempInstruction=programCounter; /* temporaere speicherung programCounter  */
      for(programCounter=0;programCounter<numberInstructions;programCounter++){
	      printProgram(programPointer);
      }

      printf("--- end of code ---\n");
      programCounter=tempInstruction;
      printProgram(programPointer);
    }else if(strcmp(inputString,"s")==0){ /* naechster programcode wird ausgegeben und ausgefuehrt */
      program(programPointer);
      programCounter++;
      printProgram(programPointer);
    }else if(strcmp(inputString,"r")==0){ /* program laeuft normal ab */
      for(;programCounter<numberInstructions;programCounter++){
        program(programPointer);
      }
      break;
    }else if(strcmp(inputString,"q")==0){ /* beenden des debugers */
      break;
    }else{
      printf("Command unknown, try again\n");
    }
  }
}


/* Gibt das Programm aus */
void printProgram(unsigned int *code){
  int zeile;
  
  zeile = (code[programCounter]&0xFF000000)>>24;
  
  if(zeile==HALT){
    printf("%03d: halt\n",programCounter);
  }else if(zeile==PUSHC){
    printf("%03d: pushc %5d\n", programCounter, (SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
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
    printf("%03d: asf %7d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==RSF){
    printf("%03d: rsf\n",programCounter);
  }else if(zeile==PUSHL){
    printf("%03d: pushl %5d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==POPL){
    printf("%03d: popl %6d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==EQ){
    printf("%03d: eq \n",programCounter);
  }else if(zeile==NE){
    printf("%03d: ne \n",programCounter);
  }else if(zeile==LT){
    printf("%03d: lt \n",programCounter);
  }else if(zeile==LE){
    printf("%03d: le \n",programCounter);
  }else if(zeile==GT){
    printf("%03d: gt \n",programCounter);
  }else if(zeile==GE){
    printf("%03d: ge \n",programCounter);
  }else if(zeile==JMP){
    printf("%03d: jmp %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==BRF){
    printf("%03d: brf %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==BRT){
    printf("%03d: brt %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==CALL){
    printf("%03d: call %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==RET){
    printf("%03d: ret\n",programCounter);
  }else if(zeile==DROP){
    printf("%03d: drop %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==PUSHR){
    printf("%03d: pushr\n",programCounter);
  }else if(zeile==POPR){
    printf("%03d: popr\n",programCounter);
  }else if(zeile==DUP){
    printf("%03d: dup\n",programCounter);
  }
}

/* ausfuehren der Instruktionen */
void program(unsigned int *code){
  int instruction, n1, n2, eingeleseneZahl;
  instruction=(code[programCounter]&0xFF000000)>>24;

  switch(instruction){
    case HALT: /* beendet das Programm */
      printf("Ninja Virtual Machone stopped\n");
      exit(0);
    case PUSHC:
      push(code[programCounter]);
      break;
    case ADD:
      n1=pop();
      n2=pop();
      push(n1+n2);
      break;
    case SUB:
      n1=pop();
      n2=pop();
      push(n2-n1);
      break;
    case MUL:
      n1=pop();
      n2=pop();
      push(n1*n2);
      break;
    case DIV:
      n1=pop();
      n2=pop();
    
      if(n2!=0){
        push(n1/n2);
      }else{
        printf("Division by Zero not possible. Programm will be stopped.\n");
        exit(-99);
      }
      break;
    case MOD:
      n1=pop();
      n2=pop();
      push(n1%n2);
      break;
    case RDINT: /* liest Zahl auf der konsole ein */
      scanf("%d", &eingeleseneZahl);
      push(eingeleseneZahl);
      break;
    case WRINT:
      printf("%d\n",SIGN_EXTEND(IMMEDIATE(stack[stackPointer-1])));
      pop();
      break;
    case ASF: /*stack frame anlegen, mit groesse n*/
      push(framePointer); /*position des letzten frames wird gemerkt*/
      framePointer=stackPointer;
      stackPointer=stackPointer+(IMMEDIATE(code[programCounter]));
      break;
    case RSF: /*benutzen stack frame entfernen*/
      stackPointer=framePointer;
      framePointer=pop(); /*position des letzten frames wird zurueckgeschrieben*/
      break;
    case PUSHL: /*variable von frame wird in stack geschrieben*/
      n1=popFrame(code[programCounter]);
      push(n1);
      break;
    case POPL: /*wert vom stack wird in frame geschrieben*/
      n1=pop();
      pushFrame(n1,code[programCounter]);
      break;
    case EQ:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction));
      break;
    case NE:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction));
      break;
    case LT:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction));
      break;
    case LE:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction));
      break;
    case GT:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction));
      break;
    case GE:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction));
      break;
    case JMP:
      programCounter = IMMEDIATE(code[programCounter])-1; /* -1 wegen for-schleifen ++ */
      break;
    case BRF:
      n1=pop();
      if(n1 == 0){
        if(numberOfCommands<(IMMEDIATE(code[programCounter])-1)){
        	printf("Branch address out of range. Programm will be stoped.");
        	exit(-99);
        }
        programCounter = IMMEDIATE(code[programCounter])-1; /* -1 wegen for-schleifen ++ */
      }else if(n1 == 1){
      /* nix */	
      }else{
        printf("Error in BRF. Stack element is neither 0 nor 1.");
        exit(-99);	
      }
      break;
    case BRT:
      n1=pop();
      if(n1 == 1){
        if(numberOfCommands<(IMMEDIATE(code[programCounter])-1)){
	        printf("Branch address out of range. Programm will be stoped.");
        	exit(-99);
        }
        programCounter = IMMEDIATE(code[programCounter])-1; /* -1 wegen for-schleifen ++ */
      }else if(n1 == 0){
        /* nix */	
      }else{
        printf("Error in BRT. Stack element is neither 0 nor 1.");
        exit(-99);	
      }
      break;
    case CALL:
      push(programCounter); /* programCounter wird zwischen gespeichert, fuer spaetere ruecksprung */
      programCounter=IMMEDIATE(code[programCounter])-1;
      break;
    case RET:
      programCounter=pop();
      break;
    case DROP: /* loescht n Werte aus dem Stack */
      n1 = IMMEDIATE(code[programCounter]);
      while(n1 > 0){
        pop();
        n1--;
      }
      break;
    case PUSHR: /* holt Wert aus returnRegister */
      push(returnRegister);
      break;
    case POPR: /* speichert Wert in returnRegister */
      returnRegister = pop();
      break;
    case DUP: /* dupliziert obersten Wert in dem Stack */
      n1 = pop();
      /*n2 = n1;*/
      push(n1);
      push(n1);
  }
}


/* Vergleicht zwei Zahlen */
int compare(int n1, int n2, int instruction){
  bool result = false;
  switch(instruction) {
    case EQ: result = (n1 == n2);
      break;
    case NE: result = (n1 != n2);
      break;
    case LT: result = (n1 < n2);
      break;
    case LE: result = (n1 <= n2);
      break;
    case GT: result = (n1 > n2);
      break;
    case GE: result = (n1 >= n2);
      break;  
  }

  return (result) ? 1 : 0;
}


/*push und pop funktionen fuer frame stack*/
void pushFrame(int num, int point){
  int i = framePointer+(SIGN_EXTEND(IMMEDIATE(point)));
  /*pruefen frame innerhalb des stacks*/
  /*if(i < stackSize){
    printf("Frameposition out of Range. Program will be stopped.\n");
    exit(-99);
  }*/
  stack[i]=num;
}
int popFrame(int point){
  int i=framePointer+(SIGN_EXTEND(IMMEDIATE(point)));
  /*pruefen ob frame inhalt innerhalb des stacks*/
  /*if(i > 0){
    printf("Frameposition out of Range. Program will be stopped.\n");
    exit(-99);
  }*/
  return stack[i];
}


void push(int num){
  stack[stackPointer]=SIGN_EXTEND(IMMEDIATE(num));/*(num&0x00FFFFFF);*/
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

/* Ninja Virtual Machine */

/* include libs */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "njvm.h"


const char version[] = "1.0beta";

/*int stack[stackSize];*/
StackItem stack[stackSize];

StackItem returnRegister;
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
    error("No Arguments, try --help.");
  }

   return  0;
}


/* open a bin-file */
void openFile(int i,int argc, char *argv[]){
  codeFile = fopen(argv[i], "r");
  if(codeFile == NULL || (strstr(argv[i], ".bin") == NULL )){
    printf("Couldn't open %s . You need to use a .bin file. Try again. \n", argv[i]);
  }else{
    /* Zeiger ans Ende setzen, um Größe zu ermitteln */
    fseek(codeFile, 0, SEEK_END);
    fileSize = ftell(codeFile);
    /* Zeiger wieder an den Anfang setzen */
    fseek(codeFile, 0, SEEK_SET);
    programPointer = malloc(fileSize);
    if( programPointer == NULL){
      error("No RAM available.");
    }
    /*printf("Size: %d Bytes \n\n", fileSize);*/
    numberOfCommands = fread(programPointer, 4, fileSize/4, codeFile);
    if(numberOfCommands < fileSize/sizeof(programPointer[0])){
      error("There occured an error reading the file.");
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
  unsigned int sizeArray, i;
  char inputString[5];

  numberInstructions=fileSize/sizeof(programPointer[0]);
  programCounter=0;

  printf("Ninja Virtual Machine started\n");
  printProgram(programPointer);

  while(programCounter<numberInstructions){
    printf("DEBUG: i (inspect), l (list), s (step), r (run), q (quit)?: ");
    scanf("%s",inputString);

    if(strcmp(inputString,"i")==0){
      printf("DEBUG, inspect: s (stack), o (object) ?: ");
      scanf("%s", inputString);
			
      if(strcmp(inputString, "s")== 0){
      	for(j=stackPointer;j>-1;j--){ /* stack von oben nach unten durchgehen */
	  if(j==stackPointer && j==framePointer){
	    printf("sp, fp --->  0x%08x: (xxxxxx) xxxx\n",j);
	  }else if(j==stackPointer){
	    printf("sp ------->  0x%08x: (xxxxxx) xxxx\n",j);
	  }else{
	    char * ausgabeString;
	    if(j==framePointer){
	      ausgabeString = "fp -------> ";
	    }else{
	      ausgabeString = "            ";
	    }

	    printf("%s 0x%08x: (%s): ",ausgabeString, j, getTypeOfVariable(j));
	    if(stack[j].isNumber == true){
	      printf("0x%08x\n", stack[j].u.number);
	    }else{
	      if(getHeapAddress(j) == NULL){
		printf("(nil)\n");
	      }else{
		printf("%p\n", getHeapAddress(j));
	      }
	    }
	  }
      	}
      	printf("--- bottom of stack ---\n");	
      }else if(strcmp(inputString, "o")== 0){
	
	ObjRef temp;

	printf("object reference? 0x");
	scanf("%p", (void **) &temp);


  printf("VMT: 0x%08x\n", temp->vmt);

	/* print obj */
	if(OBJ_HAS_BYTES(temp)){
	  for(i=0;i<4;i++){
	    printf("byte[%d] = 0x%02x\n", i, temp->data.byte[i]);
	  }
	}
	if(OBJ_HAS_OBJREF(temp)){
	  sizeArray = COUNT_FROM_OBJREF(temp);
          printf("object holds %d fields\n", sizeArray);
	  for(i=0;i<sizeArray;i++){
	    if(temp->data.field[i]!=NULL){
	      printf("field[%d] = %p\n",i,(void *)temp->data.field[i]);
	    }else{
	      printf("field[%d] = nil\n",i);
	    }
	  }
	}
	printProgram(programPointer);						
      }
    }else if(strcmp(inputString,"l")==0){
      tempInstruction=programCounter; /* temporaere speicherung programCounter  */
      for(programCounter=0;programCounter<numberInstructions;programCounter++){
        if(programCounter == tempInstruction){
          printf("\033[31m");
          printProgram(programPointer);
          printf("\033[0m"); 
        }else{
	  printProgram(programPointer);
	}
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
  int zeile, index, anzahlArgumente;
  
  zeile = (code[programCounter]&0xFF000000)>>24;



  
  if(zeile==HALT){
    /* ganze Zeile ueberpruefen um festzustellen, ob es wirklich ein HALT ist,
       oder ob es eine .addr der VMT ist*/
    if((code[programCounter]&0xFFFFFFFF) == 0){
        printf("0x%08x: halt\n",programCounter);
    }else{
        printf("0x%08x: ???\n",programCounter);
    }
  }else if(zeile==PUSHC){
    printf("0x%08x: pushc %5d\n", programCounter, (SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==ADD){
    printf("0x%08x: add\n",programCounter);
  }else if(zeile==SUB){
    printf("0x%08x: sub\n",programCounter);
  }else if(zeile==MUL){
    printf("0x%08x: mul\n",programCounter);
  }else if(zeile==DIV){
    printf("0x%08x: div\n",programCounter);
  }else if(zeile==MOD){
    printf("0x%08x: mod\n",programCounter);
  }else if(zeile==RDINT){
    printf("0x%08x: rdint\n",programCounter);
  }else if(zeile==WRINT){
    printf("0x%08x: wrint\n",programCounter);
  }else if(zeile==ASF){
    printf("0x%08x: asf %7d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==RSF){
    printf("0x%08x: rsf\n",programCounter);
  }else if(zeile==PUSHL){
    printf("0x%08x: pushl %5d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==POPL){
    printf("0x%08x: popl %6d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==EQ){
    printf("0x%08x: eq \n",programCounter);
  }else if(zeile==NE){
    printf("0x%08x: ne \n",programCounter);
  }else if(zeile==LT){
    printf("0x%08x: lt \n",programCounter);
  }else if(zeile==LE){
    printf("0x%08x: le \n",programCounter);
  }else if(zeile==GT){
    printf("0x%08x: gt \n",programCounter);
  }else if(zeile==GE){
    printf("0x%08x: ge \n",programCounter);
  }else if(zeile==JMP){
    printf("0x%08x: jmp %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==BRF){
    printf("0x%08x: brf %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==BRT){
    printf("0x%08x: brt %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==CALL){
    printf("0x%08x: call %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==RET){
    printf("0x%08x: ret\n",programCounter);
  }else if(zeile==DROP){
    printf("0x%08x: drop %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==PUSHR){
    printf("0x%08x: pushr\n",programCounter);
  }else if(zeile==POPR){
    printf("0x%08x: popr\n",programCounter);
  }else if(zeile==DUP){
    printf("0x%08x: dup\n",programCounter);
  }else if(zeile==NEW){
    printf("0x%08x: new %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==GETF){
    printf("0x%08x: getf %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==PUTF){
    printf("0x%08x: putf %2d\n",programCounter,(SIGN_EXTEND(code[programCounter]&0x00FFFFFF)));
  }else if(zeile==NEWA){
    printf("0x%08x: newa\n",programCounter);
  }else if(zeile==GETLA){
    printf("0x%08x: getla\n",programCounter);
  }else if(zeile==GETFA){
    printf("0x%08x: getfa\n",programCounter);
  }else if(zeile==PUTFA){
    printf("0x%08x: putfa\n",programCounter);
  }else if(zeile==PUSHN){
    printf("0x%08x: pushn\n",programCounter);
  }else if(zeile==REFEQ){
    printf("0x%08x: refeq\n",programCounter);
  }else if(zeile==REFNE){
    printf("0x%08x: refne\n",programCounter);
  }else if(zeile==VMCALL){
    anzahlArgumente = (code[programCounter]&0x00FF0000)>>16;
    index = (code[programCounter]&0x0000FFFF);
    printf("0x%08x: vmcall %2d,%2d\n", programCounter, anzahlArgumente, index);
  }
}

/* ausfuehren der Instruktionen */
void program(unsigned int *code){
  int instruction, eingeleseneZahl;
  unsigned int n1, n2;
  int index, numberOfArgs;
  Object *n1Object, *n2Object;

  instruction=(code[programCounter]&0xFF000000)>>24;

  switch(instruction){
    case HALT: /* beendet das Programm */
      printf("Ninja Virtual Machine stopped\n");
      exit(0);
    case PUSHC:
      push(SIGN_EXTEND(code[programCounter]&0x00FFFFFF), false);
      break;
    case ADD:
      n1=pop();
      n2=pop();
      push(n1+n2, false);
      break;
    case SUB:
      n1=pop();
      n2=pop();
      push(n2-n1, false);
      break;
    case MUL:
      n1=pop();
      n2=pop();
      push(n1*n2, false);
      break;
    case DIV:
      n1=pop();
      n2=pop();
    
      if(n1!=0){
        push(n2/n1, false);
      }else{
        error("Division by Zero not possible. Programm will be stopped.");
      }
      break;
    case MOD:
      n1=pop();
      n2=pop();
      
      if(n1!=0){
	push(n2%n1, false);
      }else{
         error("Modulo by Zero not possible. Programm will be stopped.");
      }
      break;
    case RDINT: /* liest Zahl auf der konsole ein */
      scanf("%d", &eingeleseneZahl);
      push(eingeleseneZahl, false);
      break;
    case WRINT:
      printf("%d\n",getStackVal(stackPointer-1));/*stack[stackPointer-1]);*/
      pop();
      break;
    case ASF: /*stack frame anlegen, mit groesse n*/
      push(framePointer, true); /*position des letzten frames wird gemerkt*/
      framePointer=stackPointer;
      stackPointer=stackPointer+(IMMEDIATE(code[programCounter]));
      nullStackFrame(framePointer, stackPointer);
      break;
    case RSF: /*benutzen stack frame entfernen*/
      stackPointer=framePointer;
      framePointer=pop(); /*position des letzten frames wird zurueckgeschrieben*/
      break;
    case PUSHL: /*variable von frame wird in stack geschrieben*/
      n1Object=popFrame(code[programCounter]);
      stack[stackPointer].isNumber = false;
      stack[stackPointer].u.objRef=n1Object;
      stackPointer++;
      /*push(n1, false);Ü*/
      break;
    case POPL: /*wert vom stack wird in frame geschrieben*/
      stackPointer--;
      pushFrame(stack[stackPointer].u.objRef, code[programCounter]);
      break;
    case EQ:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction), false);
      break;
    case NE:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction), false);
      break;
    case LT:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction), false);
      break;
    case LE:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction), false);
      break;
    case GT:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction), false);
      break;
    case GE:
      n1=pop();
      n2=pop();
      push(compare(n2, n1, instruction), false);
      break;
    case JMP:
      programCounter = IMMEDIATE(code[programCounter])-1; /* -1 wegen for-schleifen ++ */
      break;
    case BRF:
      n1=pop();
      if(n1 == 0){
        if(numberOfCommands<(IMMEDIATE(code[programCounter]))){
                error("Branch address out of range. Programm will be stopped.");
        }
        programCounter = IMMEDIATE(code[programCounter])-1; /* -1 wegen for-schleifen ++ */
      }else if(n1 == 1){
      /* nix */	
      }else{
        error("Error in BRF. Stack element is neither 0 nor 1.");
      }
      break;
    case BRT:
      n1=pop();
      if(n1 == 1){
        if(numberOfCommands<(IMMEDIATE(code[programCounter]))){
            error("Branch address out of range. Programm will be stopped.");
        }
        programCounter = IMMEDIATE(code[programCounter])-1; /* -1 wegen for-schleifen ++ */
      }else if(n1 == 0){
        /* nix */	
      }else{
        error("Error in BRT. Stack element is neither 0 nor 1.");
      }
      break;
    case CALL:
      push(programCounter+1, true); /* programCounter wird zwischen gespeichert, fuer spaetere ruecksprung */
      programCounter=IMMEDIATE(code[programCounter])-1;
      break;
    case RET:
      programCounter=pop()-1;
      break;
    case DROP: /* loescht n Werte aus dem Stack */
      n1 = IMMEDIATE(code[programCounter]);
      /*objRef = newStackVal(true, n1);*/
      while(n1 > 0){
        pop();
        n1--;
      }
      break;
    case PUSHR: /* holt Wert aus returnRegister */
      stack[stackPointer] = returnRegister;
      stackPointer++;
      break;
    case POPR: /* speichert Wert in returnRegister */
      stackPointer--;
      returnRegister = stack[stackPointer];
      break;
    case DUP: /* dupliziert obersten Wert in dem Stack */
      if(stack[stackPointer-1].isNumber){
          error("topObjRef detected number on top of stack");
      }
      stack[stackPointer].isNumber = false;
      stack[stackPointer].u.objRef = getHeapAddress(stackPointer-1);
      stackPointer++;
      break;
    case NEW:
      n1 = IMMEDIATE(code[programCounter]);
      stack[stackPointer].u.objRef = newStackVal(true, n1);
      stack[stackPointer].isNumber = false;
      programCounter++;
      stack[stackPointer].u.objRef->vmt = code[programCounter];
      stackPointer++;
      break;
    case GETF:
      n1 = IMMEDIATE(code[programCounter]);
      n1Object = getf(n1);
      stack[stackPointer].isNumber = false;
      stack[stackPointer].u.objRef = n1Object;
      stackPointer++;
      break;
    case PUTF:
      n1Object = popObject();
      n2Object = popObject();
      putf(IMMEDIATE(code[programCounter]), n1Object, n2Object);
      break;
    case NEWA:
      n1 = pop();
      stack[stackPointer].u.objRef = newStackVal(true, n1);
      stack[stackPointer].isNumber = false;
      programCounter++;
      stack[stackPointer].u.objRef->vmt = code[programCounter];
      stackPointer++;
      break;
    case GETLA:
      n1Object = popObject();
      n1 = COUNT_FROM_OBJREF(n1Object);
      push(n1, false);
      break;
    case GETFA:
      n1 = pop();
      n1Object = getf(n1);
      stack[stackPointer].isNumber = false;
      stack[stackPointer].u.objRef = n1Object;
      stackPointer++;
      break;
    case PUTFA:
      n1Object = popObject();
      n1 = pop();
      n2Object = popObject();
      if(n1<0 || n1>=COUNT_FROM_OBJREF(n2Object)){
        error("index out of bounds exception");
      }
      putf(n1, n1Object, n2Object);
      break;
    case PUSHN:
      stack[stackPointer].isNumber = false;
      stack[stackPointer].u.objRef = NULL;
      stackPointer++;
      break;
    case REFEQ:
      n1Object = popObject();
      n2Object = popObject();
      if(n1Object == n2Object){
	push(1, false);
      }else{
	push(0, false);
      }

      break;
    case REFNE:
      n1Object = popObject();
      n2Object = popObject();
      if(n1Object != n2Object){
	push(1, false);
      }else{
	push(0, false);
      }
      break;
    case VMCALL:
      index = INDEX(code[programCounter]);
      numberOfArgs = NUMBER_OF_ARGS(code[programCounter]);
      /* Return-Adresse sichern, indem sie auf den Stack gelegt wird */
      push(programCounter+1, true);
      /* programCounter auf die naechste Anweisung setzen */
      /* -1 weil der programCounter beim naechsten Durchlauf wieder um 1 erhoeht
         wird*/
      programCounter = code[stack[stackPointer-(numberOfArgs+1)].u.objRef->vmt+index]-1;
      break;
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
void pushFrame(Object *object, int point){
  int i = framePointer+(SIGN_EXTEND(IMMEDIATE(point)));
  stack[i].isNumber = false;
  stack[i].u.objRef = object;
}
Object *popFrame(int point){
  int i=framePointer+(SIGN_EXTEND(IMMEDIATE(point)));
  return stack[i].u.objRef;
  /*return getStackVal(i);*/
}


void push(int num, bool isNumber){
  Object *objRef;

  if(isNumber){
    stack[stackPointer].isNumber = true;
    stack[stackPointer].u.number = num;
  }else{
    objRef = newStackVal(false, 1);
    *(int *)&objRef->data.byte[0]=num;
    stack[stackPointer].isNumber = false;
    stack[stackPointer].u.objRef = objRef;
  }

  stackPointer++;
  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  if(stackPointer > stackSize){
      error("Stackposition out of Range. Program will be stopped.");
  }
}

int pop(void){
  stackPointer--;

  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  if(stackPointer < 0){
      error("Stackposition out of Range. Program will be stopped.");
  }

  return getStackVal(stackPointer);
}

Object *popObject(void){
  stackPointer--;
  return stack[stackPointer].u.objRef;
}

Object *getf(int index){
  Object *objRef;

  /* Überprüfen, ob die Position innerhalb des Stacks liegt */
  stackPointer--;
  objRef = stack[stackPointer].u.objRef;
  /*printf("Adresse von objRef: %x\n", (int) *objRef);*/
  if(stackPointer < 0){
    error("Stackposition out of Range. Program will be stopped.");
  }

  if(index >= (COUNT_FROM_OBJREF(objRef)) || index < 0){
    error("index out of bounds exception");
  }

  return objRef->data.field[index];
}

/*--- putf ---*/
void putf(int index, Object *objValue, Object *objRef){

  if(index >= (COUNT_FROM_OBJREF(objRef)) || index < 0){
    error("index out of bounds exception");
  }

  objRef->data.field[index] = objValue;
}

Object *newStackVal(bool isObject, int size){
  Object *objRef;
  int i;

  /* speicher freigeben */
  if(!isObject){
    objRef = malloc(sizeof(Object)-sizeof(Data)+size*sizeof(int));
    if(objRef==NULL){
      error("no memory");
    }

    objRef->size = (unsigned int)(1*sizeof(unsigned int) | MSB );
    for(i=0; i<size; i++){
      objRef->data.byte[i] = 0;
    }
    objRef->vmt = 0;
  }else{
    objRef = malloc(sizeof(Object)-sizeof(Data)+size*sizeof(objRef));
    if(objRef==NULL){
      error("no memory");
    }
    objRef->size = size;
    for(i=0; i<size; i++){
      objRef->data.field[i] = NULL;
    }
  }

  return objRef;
}

int getStackVal(int i){
  if(stack[i].isNumber){
    return stack[i].u.number;
  }else{
    return *(int*)stack[i].u.objRef->data.byte;
  }
}

char *getTypeOfVariable(int j){
  if(stack[j].isNumber == true){
    return "number";
  }else{
    return "objref";
  }
}



void *getHeapAddress(int i){
  return stack[i].u.objRef;
}

void nullStackFrame(int frameP, int stackP){
    while(stackP > frameP){
        stack[frameP].isNumber = false;
        stack[frameP].u.objRef = NULL;
        frameP++;
    }
}

void error(char *errMsg){
    printf("Error: %s\n", errMsg);
    exit(-99);
}



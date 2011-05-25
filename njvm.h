

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

void printHelp(void);
void program(unsigned int *code);
void printProgram(unsigned int *code);
void push(int num);
int pop(void);
void pushFrame(int num, int point);
int popFrame(int point);
int compare(int n1, int n2, int instruction);
void openFile(int i, int argc, char *argv[]);
void closeFile(void);
void debug(void);

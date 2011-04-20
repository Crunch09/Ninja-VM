void printHelp(void);
void program(unsigned int *code, int size);
void printProgram(unsigned int *code, int size);
void push(int num);
int pop(void);
void pushFrame(int num, int point);
int popFrame(int point);
int compare(int n1, int n2, int instruction);

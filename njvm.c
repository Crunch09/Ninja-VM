#include <stdio.h>
#include <string.h>

const char version[] = "0.1";

void printHelp(void);


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
			  printf("unknown command line argument '%s', try './njvm --help' \n", argv[1]);
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

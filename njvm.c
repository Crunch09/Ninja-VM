#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	
	if(argc >= 2){
		/* Hilfe ausgeben */
		if(strcmp(argv[1], "--help") == 0){
			printf("usage: ./njvm [option] [option] ... \n");
			printf("      --version        show version and exit\n");
			printf("      --help           show this help and exit\n");
		}else if(strcmp(argv[1], "--version") == 0){
			/* Versionsinformationen ausgeben */
			printf("Ninja Virtual Machine version 0 ( compiled %s, %s )\n", __DATE__, __TIME__);
		}else{
			/* Unbekannter Befehl */
			printf("unknown command line argument '%s', try './njvm --help' \n", argv[1]);
		}
	}else{
		/* Keine Parameter angegeben */
		printf("Ninja Virtual Machine started\n");
		printf("Ninja Virtual Machine stopped\n");
	}
	
	
	return 0;
}
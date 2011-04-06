#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	
	if(argc >= 2){
		if(strcmp(argv[1], "--help") == 0){
			printf("usage: ./njvm [option] [option] ... \n");
			printf("      --version        show version and exit\n");
			printf("      --help           show this help and exit\n");
		}else if(strcmp(argv[1], "--version") == 0){
			printf("Ninja Virtual Machine version 0 ( compiled %s, %s )\n", __DATE__, __TIME__);
		}else{
			printf("unknown command line argument '%s', try './njvm --help' \n", argv[1]);
		}
	}else{
		printf("Ninja Virtual Machine started\n");
		printf("Ninja Virtual Machine stopped\n");
	}
	
	
	return 0;
}
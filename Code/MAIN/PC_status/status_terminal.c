#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
/*
char* readFile(char* filename)
{
	FILE* file = fopen(filename,"r");
	if(file == NULL)
	{
		return NULL;
     	}

     fseek(file, 0, SEEK_END);
     long int size = ftell(file);
     rewind(file);

     char* content = calloc(size + 1, 1);

     fread(content,1,size,file);

     return content;
     //return NULL;
}*/

int print_PC_read_errors(){
	int max_size = 20, i;
	char line[2048];
	char message[1024];
	
	system("clear");

	//fp = readFile("last_errors.txt");
	FILE* file = fopen("last_errors.txt","r");
	if(file == NULL)
	{
		//exit(-1);
     	}

	while(fgets(line, sizeof(line), file) != NULL){
		if(i == max_size){
			break;
		}else{
			i++;		
		}

		if(sscanf(line, "%s", message) != 1){
			printf("Wrong line read\n");			
			//exit(-1);	
		}

		//printf("%s",message);

		if(strcmp(message,"-x") == 0){
			fclose(file);
			return 1;		
		}

		printf("%s\n",message);
	}

	fclose(file);
	return 0;
}

int main(){
	int check = 0;

	while(check == 0){
		sleep(1); //sleep 50ms
		check = print_PC_read_errors();

		if(check == 1){
			break;		
		}

		
	}
	return 0;
}

#include <stdio.h>
// #include <stdlib.h>

// char* readFile(char* filename)
// {
    // FILE* file = fopen(filename,"r");
    // if(file == NULL)
    // {
        // return NULL;
    // }

    // fseek(file, 0, SEEK_END);
    // long int size = ftell(file);
    // rewind(file);

    // char* content = calloc(size + 1, 1);

    // fread(content,1,size,file);

    // return content;
    // //return NULL;
// }


// void safe_measurement_to_file(char* filename, char* t_string){
	// //int sz;
	// //char* buff_string;

	// FILE *fp;
	// fp = fopen(filename, "aw+");

	// /*fseek(fp, 0L, SEEK_END);
	// sz = ftell(fp);
	// fseek(fp, 0L, SEEK_SET);
	// buff_string = malloc(sz);
// */
	// fprintf(fp, "%s", t_string);

 	// fclose(fp);
// }

// void PC_log_errors(char* t_string){
	// safe_measurement_to_file("last_errors.txt", t_string);
// }

// char** PC_read_errors(){
	// int max_size = 20, i;
	// char* t_arr[max_size];
	// char* file_outcome;

	// fp = readFile("last_errors.txt");

	// for(i = 0; i < max_size; i++){
		// t_arr[i] = malloc(150);
	// }
// }

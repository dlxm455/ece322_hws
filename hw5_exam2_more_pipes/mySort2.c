#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// read from stdin to internal array
int read_stdin_to_arr(char * data[], int arr_size) {
	char str[128];
	int count = 0;
	int len;
	while((count < arr_size) && (scanf("%s", str) == 1)) {
		len = strlen(str);
		data[count] = (char *)malloc(len+1); // leave one byte space for '\0'
		strcpy(data[count], str);
        data[count][len] = '\0';
		count++;	
	}
	return count;
}

// write from internal array to stdout
int write_arr_to_stdout(char * data[], int arr_size) {
	int i;
	for (i = 0; i < arr_size; i++) {
		printf("%s\n", data[i]);
		//fflush(stdout);
	}
	return i;
}

// comparison function for qsort()
int cmpfunc(const void * a, const void * b) {
	return strcmp(*(char **)a, *(char **)b);
}

int main() {
    
    // read in number from stdin
	int num;
	if (scanf("%d", &num) != 1) {
		fprintf(stderr, "read fails");
		exit(1);
	}

    // alloc space for array of char* in heap
	char ** data = (char **)malloc(sizeof(char *) * num);
    
    // read data from stdin to array
	int read_num = read_stdin_to_arr(data, num);
    
	if (read_num != num) { // check if all data have been read
		fprintf(stderr, "read fails in mySort2");
		exit(1);
	}
    
    // quick sort
	qsort(data, num, sizeof(char*), cmpfunc);
 
    // write sorted data from array to stdout
	int write_num = write_arr_to_stdout(data, num);
	
	if (write_num != num) { // check if all data have been written
		fprintf(stderr, "write fails in mySort2");
		exit(1);
	}
    
    // free memories allocated for array and each element
    int i;
    for (i = 0; i < num; i++) {
        free(data[i]);
    }
    
    free(data);
    
}
		
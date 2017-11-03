#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// read from stardard input; returns how many stirngs have been read
int readFileToArr(char **data, int arr_size) {
	char str[128];
	int count = 0;
	int len;
	while(count < arr_size && scanf("%s", str)) {
		len = strlen(str);
		data[count] = (char *)malloc(len+1); // leave one byte space for '\0'
		strcpy(data[count], str);
		count++;	
	}
	return count;
}

int writeArrToFile(char ** data, char * fname, int arr_size) {
	FILE *outfile = fopen(fname, "w");
	int i;
	for (i = 0; i < arr_size; i++) {
		fprintf(outfile, "%s\n", data[i]);
		fflush(outfile);
	}
	fclose(outfile);
	return i;
}

// comparison function for qsort()
int cmpfunc(const void *a, const void *b) {
	return strcmp(*(char **)a, *(char **)b);
}



int main(int argc, char* argv[]) {
	int num = atoi(argv[1]);
	char ** data = (char **)malloc(sizeof(char *) * num);
	int read_num = readFileToArr(data, num);
	qsort(data, read_num, sizeof(char*), cmpfunc);
	writeArrToFile(data, argv[2], read_num);

	// free memory
	int i;
	for (i = 0; i < num; i++) {
		free(data[i]);
	}
	free(data);

	return 0;
}


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// read from stardard input; returns how many stirngs have been read
int readInData(char **data, int num_str) {
	char str[128];
	int count = 0;
	int len;
	while(count < num_str && scanf("%s", str)) {
		len = strlen(str);
		data[count] = (char *)malloc(len+1); // leave one byte space for '\0'
		strcpy(data[count], str);
		count++;	
	}
	return count;
}

int writeToFile(char ** data, char * fname, int num_str) {
	FILE *outfile = fopen(fname, "w");
	int i;
	for (i = 0; i < num_str; i++) {
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
	int num_string = atoi(argv[1]);
	char ** data = (char **)malloc(sizeof(char *) * num_string);
	int read_num_str = readInData(data, num_string);
	qsort(data, read_num_str, sizeof(char*), cmpfunc);
	writeToFile(data, argv[2], read_num_str);

	// free memory
	int i;
	for (i = 0; i < num_string; i++) {
		free(data[i]);
	}
	free(data);

	return 0;
}
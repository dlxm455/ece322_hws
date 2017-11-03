#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readFileToArr(char **data, int arr_size) {
	char buf[128]; // buffer for scanf 
	int count = 0;
	int len; // length for each string read from stdin

	while (count < arr_size && scanf("%s", buf)) {
		len = strlen(buf);
		data[count] = (char *)malloc(len+1); // leave one byte space for '\0'
		strcpy(data[count], buf);
		count++;
	}
	return count;
}

int writeArrToFile(char * ptr, int size, FILE * outfile) {
	int i;
	for (i = 0; i < size; i++) {
		fprintf(outfile, "%s\n", ptr+i);
		fflush(outfile);
	}
	return i;
} 

int main(int argc, char*argv[]) {
	int num1 = atoi(argv[1]);
	int num2 = atoi(argv[2]);
	
	char **data1 = (char **)malloc(sizeof(char *) * num1);
	char **data2 = (char **)malloc(sizeof(char *) * num2);

	int read_num1 = readFileToArr(data1, num1);
	int read_num2 = readFileToArr(data2, num2);
	
	FILE * outfile = fopen(argv[3], "w");	

	int i = 0, j = 0;
	while (i < read_num1 || j < read_num2) {
		if ( i == read_num1) {
				writeArrToFile(data2[j], read_num2 - j, outfile);
				break;
		}
		else if (j == read_num2) {
				writeArrToFile(data1[i], read_num1 - i, outfile);
				break;
		}
			
		int cmpRes = strcmp(data1[i], data2[j]);
		if (cmpRes < 0) {
			writeArrToFile(data1[i], 1, outfile);
			i++;
		}
		else {
			writeArrToFile(data2[j], 1, outfile);
			j++;
		}
	}
	
	fclose(outfile);
	
	// free memory
	for (i = 0; i < num1; i++) {
		free(data1[i]);
	}
	free(data1);

	for (j = 0; j < num2; j++) {
		free(data2[j]);
	}
	free(data2);

	return 0;

}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read data from stdin to internal array
void read_stdin_to_arrs(char * data1[], int arr1_size, char * data2[], int arr2_size) {
	char str[128];
	int code;
	int count1 = 0;
	int count2 = 0;
	int len;

	while(((count1 < arr1_size) || (count2 < arr2_size)) && (scanf("%d %s", &code, str) == 2)) {
		len = strlen(str);
		if (code == 1) {
			data1[count1] = (char *)malloc(len+1); // leave one byte space for '\0'
			strcpy(data1[count1], str);
			data1[count1][len] = '\0';
			count1++;
            
		} else if (code == 2) {
			data2[count2] = (char *)malloc(len+1); // leave one byte space for '\0'
			strcpy(data2[count2], str);
			data2[count2][len] = '\0';
			count2++;
            
		} else { // code not 1 or 2
			fprintf(stderr, "code is not 1 or 2\n");
			exit(1);
		}
	}
	fprintf(stderr, "count1: %d", count1);
	fprintf(stderr, "count2: %d", count2);
	fprintf(stderr, "arr1size: %d", arr1_size);
	fprintf(stderr, "arr2size: %d", arr2_size);	
	if (count1 != arr1_size || count2 != arr2_size) {
		fprintf(stderr, "not all data are read to myMerge2\n");
		exit(1);
	}
}

// write from internal array to stdout
void write_arr_to_stdout(char ** start_ptr, int size) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%s\n", *(start_ptr + i));
		//fflush(outfile);
    }
}

int main (int argc, char * argv[]) {
	int num1, num2;
	num1 = (int)strtol(argv[1], NULL, 10);
	num2 = (int)strtol(argv[2], NULL, 10);
	
	// allocate two arrays to store the data
	char ** data1 = (char **)malloc(sizeof(char *) * num1);
	char ** data2 = (char **)malloc(sizeof(char *) * num2);

	// read from stdin to arrays
	read_stdin_to_arrs(data1, num1, data2, num2);
	
	// merge data from arrays and write to stdout
	int i = 0, j = 0;
	while (i < num1 || j < num2) {
		if (i == num1) {
				write_arr_to_stdout(data2+j, num2 - j);
				break;
		} else if (j == num2) {
				write_arr_to_stdout(data1+i, num1 - i);
				break;
		}
			
		int cmpRes = strcmp(data1[i], data2[j]);
		if (cmpRes < 0) {
			write_arr_to_stdout(data1+i, 1);
			i++;
		} else {
			write_arr_to_stdout(data2+j, 1);
			j++;
		}
	}

	// free memories
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



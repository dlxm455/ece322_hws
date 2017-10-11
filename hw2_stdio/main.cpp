#include "my_stdio.h"

char* readStr(FILE * file) {
	int c_int = fgetc(file);
	char * char_arr = NULL;
	int i = 0;
	while (c_int != EOF) {
		char_arr[i] = char(c_int)
		if (c_int == NULL) {
			return char_arr;
		}
		i++;
	}
	return char_arr;
}

int readInt(FILE * file) {
	char* int_char_arr = readStr(file);
	int arr_size = 0;
	while (int_char_arr[arr_size] != NULL) {
		arr_size += 1;
	} 
	int i;
	int res = 0;
	for (i = 0; i < arr_size; ++i) {
		res += 	
}

void writeStr(FILE * file, char * string) {

}

void writeInt(FILE * file, int interger) {

}
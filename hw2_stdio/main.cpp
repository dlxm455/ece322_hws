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
	int i = 0;
	while (string[i] != '\0') {
		fputc(file, (int)string[i]);
	}
}

void writeInt(FILE * file, int integer) {
		//find out the number of digits in the integer
		int digit_num = 1;
		int i = 10;
		while (integer >= i) {
			i = i * 10;
			digit_num += 1;
		}
		char * string = (char*)malloc(digit_num + 1);
		string[digit_num] = '\0';
		while (digit_num > 1) {
			string[digit_num - 1] = integer % 10 + '0';
			integer /= 10;
			digit_num -= 1;
		}
		string[0] = integer;
		writeStr(file, string);
		free(string);
}
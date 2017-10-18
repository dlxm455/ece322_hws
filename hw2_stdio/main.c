#include "my_stdio.h"
#include <stdlib.h>
#include <stdio.h>



#define init_capacity 100

char * readStr(FILE * file) {
	int int_character = fgetc(file);
// if fgetc() returns error or EOF (<0) 
// or first character is control character (1 to 32 or 127) or space(32) or nbsp(255), return NULL 
	if (int_character < 33 || int_character == 127 || int_character == 255)
		return NULL:
	// malloc a space for the string with pre-defined init_capacity 
	// if needs more space, malloc a new space twice the size. copy and delete the old one
	init_capacity = init_capacity;
	char * char_arr = (char *)malloc(capacity);
	char * new_arr;
	int i = 0;
	
	// check int_character is not control character (< 32 or 127) space(32), nbsp(255)
	while (int_character >= 33 && int_character != 127 && int_character != 255) {
		if (i >= capacity) { // expand memory
			new_arr = (char *)malloc(capacity * 2);
			new_arr = memcpy(new_arr, char_arr, capacity);
			free char_arr;
			char_arr = new_arr;
			new_arr = NULL;
			capacity *= 2;
		}
		
		int_character = fgetc(file);
		char_arr[i] = (char)int_character;
		i++;
	}

	// add '\0' at the end of arr 
	// if no more space, malloc a new space with 1 more char size, copy and delete the old one
	if (i == capacity) {
		new_arr = (char *)malloc(capacity + 1);
		new_arr = memcpy(new_arr, char_arr, capacity);
		free char_arr;
		char_arr = new_arr;
		new_arr = NULL;
	}

	char_arr[i] = '\0';
	
	return char_arr;
}

int readInt(FILE * file) {
	char* char_arr_integer = readStr(file);
	int negative_flag = 0;
	// find out number of digits and check if the string is a valid integer
	int num_digit = 0;
	char temp = char_arr_integer[0];
	if (temp == '-') {
		negative_flag = 1;
		temp = char_arr_integer[1];
	}

	while (temp != '\0') {
		if ((temp < '0' || temp > '9')) { // not a number
			printf("Not an integer"); return INT_MAX; // return a big number to indicate error
		}
		num_digit += 1;
		temp = char_arr_integer[num_digit];
	}

	int i, res = 0, base = 1;
	for (i = 0; i < num_digit; i++) {
		res += (char_arr_integer[num_digit - 1 - i] - '0') * base;
		base *= 10;
	}

	if (negative_flag) res = - res;
	return res;	
}

void writeStr(FILE * file, char * string) {
	int i = 0;
	while (string[i] != '\0') {
		fputc(file, (int)string[i]);
		i++;
	}
	fputc(32, file); // add a white space
	fflush(file); // flush the string to the file
}

void writeInt(FILE * file, int integer) {
		//find out the number of digits in the integer
		int num_digit = 1;
		int base = 10;
		while (integer >= base ) {
			base *= 10;
			num_digit += 1;
		}
		char * string = (char*)malloc(digit_num + 1);
		string[num_digit] = '\0'; // add '\0' at the end
		int i = num_digit;
		while (i >= 1) {
			string[i - 1] = integer % 10 + '0';
			integer /= 10;
			i--;
		}
		writeStr(file, string);
		free(string);
}

int main(int argc, char * argv[]) {
		FILE ** file_arr = fopen(&argv[1], argc - 1, 5);
		// assume there are three files
		FILE * file0 = file_arr[0];
		fputc((int)('a'), file0);
		fputc((int)('b'), file0);
		fputc((int)('c'), file0);

		FILE * file1 = file_arr[1];
		char [] my_string = "abcdefghij";
		writeStr(file1, my_string);	
		
		FILE * file2 = file_arr[2];
		int integer = 123456; 

		writeInt(file1, integer);

}

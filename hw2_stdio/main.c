#include "my_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int readStr(FILE * file, char ** result) {
	int int_character = fgetc(file);
	if (int_character == -1) return -1;

	int capacity = 100; // initial allocation size
	*result = (char *)malloc(capacity);
	int i = 0;	
	// check int_character is not control character (< 32 or 127) space(32), nbsp(255)
	while (int_character >= 33 && int_character != 127 && int_character != 255) {
		if (i == capacity - 1) { 
			// if not enough space, allocate a twice size space, copy data and free old memory
			capacity *= 2;
			char * new_arr = (char *)malloc(capacity);
			memcpy(new_arr, result, i);
			free(result);
			*result = new_arr;
			new_arr = NULL;
		}
		*(*result+i) = (char)int_character;
		int_character = fgetc(file);
		if (int_character == -1) return -1;
		i++;
	}

	*(*result+i) = '\0';
	return 0;
}

int readInt(FILE * file, int * result) {
	char * char_arr_integer = NULL;
	
	int ret = readStr(file, &char_arr_integer);
	if (ret == -1) {
		if (char_arr_integer) free(char_arr_integer);
		return -1;
	}
	
	int neg_flag = 0;
	// find out number of digits and check if the string is a valid integer
	int num_digit = 0, total_size = 0, i = 0;
	char temp = char_arr_integer[0];
	if (temp == '-') {
		neg_flag = 1;
		i = 1;
		temp = *(char_arr_integer+i);
	}

	while (temp != '\0') {
		if ((temp < '0' || temp > '9')) { // not a number
			return -1;
		}
		num_digit++;
		i++;
		temp = *(char_arr_integer+i);
	}

	int res = 0, base = 1;
	for (i = 0; i < num_digit; i++) {
		if (neg_flag) 
			res += (*(char_arr_integer+num_digit - i) - '0') * base;
		else 
			res += (*(char_arr_integer+num_digit - 1 - i) - '0') * base;
		base *= 10;
	}

	if (neg_flag) res = - res;
	*result = res;

	if (char_arr_integer) free(char_arr_integer);
	return 0;
}

int writeStr(FILE * file, char * string) {
	int i = 0;
	int ret = 0;
	while (string[i] != '\0') {
		ret = fputc((int)string[i], file);
		if (ret == -1) return -1;
		i++;
	}
	fputc(32, file); // add a white space
	ret = fflush(file); // flush the string to the file
	return ret;
}

int writeInt(FILE * file, int integer) {
		//find out the number of digits in the integer
		int neg_flag = 0;
		int ret = 0;
		if (integer < 0) {
			neg_flag = 1;
			integer = -integer;
		}
		int num_digit = 1;
		int base = 10;
		while (integer >= base ) {
			base *= 10;
			num_digit += 1;
		}
		if (neg_flag) num_digit += 1;
		char * string = (char*)malloc(num_digit + 1);
		string[num_digit] = '\0'; // add '\0' at the end
		int i = num_digit;
		while (i >= 1) {
			string[i - 1] = integer % 10 + '0';
			integer /= 10;
			i--;
		}
		if (neg_flag) string[0] = '-';
		ret = writeStr(file, string);
		free(string);
		return ret;
}

int main(int argc, char * argv[]) {
		// test fopen
		FILE * file1 = fopen("file1.txt", "w+", 5); // read and write
		FILE * file2 = fopen("file2.txt", "w+", 5); // read and write
		FILE * file3 = fopen("file3.txt", "w", 5); // write only
		
		// test fputc and fflush 
		fputc((int)('a'), file1);
		fputc((int)('b'), file1);
		fputc((int)('c'), file1);
		fflush(file1);

		// test fgetc, ungetc
		lseek(file1->fd, 0, SEEK_SET);
		fputc(fgetc(file1), file2);
		fputc(fgetc(file1), file2);
		ungetc(-1, file1);
		ungetc((int)('z'), file1);
		fputc(fgetc(file1), file2);
		fputc(fgetc(file1), file2); 
	
		// test writeStr	
		writeStr(file2, "hello");	
		
		// test writeInt
		lseek(file1->fd, 3, SEEK_SET);	
		writeInt(file1, -123);
	
		// test fclose, readInt
		fclose(file1);
		file1 = fopen("file1.txt", "r", 5); // read only
		lseek(file1->fd, 3, SEEK_SET);
		int x;
		readInt(file1, &x);
		writeInt(file3, x);
		
		// test readStr, writeStr
		char * s = NULL;
		lseek(file2->fd, 4, SEEK_SET);
		readStr(file2, &s);
		writeStr(file3, s); 
		
		if (s) free(s);

		fclose(file1); //file1 should have abc-123 
		fclose(file2); //file2 should have abzbhello 
		fclose(file3); //file3 should have -123 hello 
	
		return 0;
}

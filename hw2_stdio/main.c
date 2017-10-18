#include "my_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int readStr(FILE * file, char * result) {
	int int_character = fgetc(file);
// if fgetc() returns error or EOF (<0) 
	int count = 0;
	
	// check int_character is not control character (< 32 or 127) space(32), nbsp(255)
	while (int_character >= 33 && int_character != 127 && int_character != 255) {
		int_character = fgetc(file);
		count++;
	}

	result = (char *)malloc(count + 1);
	lseek(file->fd, -count, SEEK_CUR);	
	int i;
	for (i = 0; i < count; i++) {
		int_character = fgetc(file);
		result[i] = (char)int_character;
		i++;
	}

	result[i] = '\0';
	return 0;
}

int readInt(FILE * file, int * result) {
	char* char_arr_integer;
	int ret = readStr(file, char_arr_integer);
	if (ret == -1) {
		if (char_arr_integer != NULL) free(char_arr_integer);
		return -1;
	}
	
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
			return -1;
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
	*result = res;

	if (char_arr_integer != NULL) free(char_arr_integer);
	return 0;
}

void writeStr(FILE * file, char * string) {
	int i = 0;
	while (string[i] != '\0') {
		fputc((int)string[i], file);
		i++;
	}
	fputc(32, file); // add a white space
	fflush(file); // flush the string to the file
}

void writeInt(FILE * file, int integer) {
		//find out the number of digits in the integer
		int neg_flag = 0;
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
		writeStr(file, string);
		free(string);
}

int main(int argc, char * argv[]) {
		FILE * file1 = fopen("file1.txt", "w+", 5);
		FILE * file2 = fopen("file2.txt", "w+", 5);
		FILE * file3 = fopen("file3.txt", "w", 5);
		// assume there are three files
		fputc((int)('a'), file1);
		fputc((int)('b'), file1);
		fputc((int)('c'), file1);
		fflush(file1);
		lseek(file1->fd, 0, SEEK_SET);

		fputc(fgetc(file1), file2);
		fputc(fgetc(file1), file2);
		
		writeStr(file2, "hello");	
		
		lseek(file1->fd, 3, SEEK_SET);	
		writeInt(file1, -123);

		/*	
		lseek(file1->fd, 3, SEEK_SET);
		int x;
		readInt(file1, &x);
		writeInt(file3, x);
		*/
		
		char * s = NULL;
		lseek(file2->fd, 2, SEEK_SET);
		readStr(file2, s);
		writeStr(file3, s); 
		
		if (s) free(s);
		

		fclose(file1); //file1 should have abc-123
		fclose(file2); //file2 should have abhello
		fclose(file3); //file3 should have -123 hello 
	
		return 0;
}

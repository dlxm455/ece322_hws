#include "my_stdio.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

// Returns an array of FILE pointers associated with each given pathname.
// Each FILE pointer points to a FILE struct if OK; the FILE pointer is NULL if error.
/*
FILE ** fopen(char ** pathname_arr, int pathname_count, int buf_size) {
	FILE ** file_arr = (FILE **)malloc(pathname_count * sizeof(FILE *));
	int i;
	for (i = 0; i < pathname_count; i++) {	
		FILE * file = (FILE *)malloc(sizeof(FILE));
		file->fd = open(pathname_arr[i], O_RDWR|O_CREAT|O_TRUNC, 0644); // 0 indicateds octal notation
		if (file->fd == -1) {
			file = NULL;
		}
		else {
			file->buf_size = buf_size;
			file->read_return_size = buf_size; // to detect EOF when reading 
			file->r_buf = NULL;
			file->w_buf = NULL;
			file->r_pos = -1; // previous reading buffer position
			file->w_pos = -1; // previous writing buffer position
		}
		file_arr[i] = file;
	}
	return file_arr;
}	
*/

// Returns: file pointer if OK, NULL on error
FILE * fopen(char * pathname, char * type, int buf_size) {
		FILE * file = (FILE *)malloc(sizeof(FILE));
		if (strcmp(type, "r") == 0 || strcmp(type, "rb") == 0)
			file->fd = open(pathname, O_RDONLY);
		else if (strcmp(type, "w") == 0 || strcmp(type, "wb") == 0) 
			file->fd = open(pathname, O_WRONLY|O_CREAT|O_TRUNC, 0644);
		else if (strcmp(type, "a") == 0 || strcmp(type, "ab") == 0) 
			file->fd = open(pathname, O_WRONLY|O_CREAT|O_APPEND, 0644);
		else if (strcmp(type, "r+") == 0 || strcmp(type, "r+b") == 0 || strcmp(type, "rb+") == 0) 
			file->fd = open(pathname, O_RDWR);
		else if (strcmp(type, "w+") == 0 || strcmp(type, "w+b") == 0 || strcmp(type, "wb+") == 0)
			file->fd = open(pathname, O_RDWR|O_CREAT|O_TRUNC, 0644);
		else if (strcmp(type, "a+") == 0 || strcmp(type, "a+b") == 0 || strcmp(type, "ab+") == 0)
			file->fd = open(pathname, O_RDWR|O_CREAT|O_APPEND, 0644);
		else
			return NULL;

		if (file->fd == -1) 
			return NULL;

		file->buf_size = buf_size;
		file->read_return_size = buf_size; // to detect EOF when reading 
		file->r_buf = NULL;
		file->w_buf = NULL;
		file->r_pos = -1; // previous reading buffer position
		file->w_pos = -1; // previous writing buffer position
	
	return file;
}



// Returns 0 if OK, -1 on error
// Writing buffer is flushed; both buffers are released; file is closed
int fclose(FILE * file) {
	int fd = file->fd;
	int res;
	free(file->r_buf);
	res = fflush(file); // returns 0 if OK, -1 on error
	if (res < 0) {
		return res;
	}
	free(file->w_buf);
	res = close(fd); // returns 0 if OK, -1 on error
	return res;
}



// Returns: next character if OK, -1 EOF or error
int fgetc(FILE * file) {
	int res;
	int buf_size = file->buf_size;

	if (file->r_buf == NULL || file->r_pos == buf_size -1) {
		if (file->r_buf == NULL)
			file->r_buf = (char *)malloc(buf_size);

		// fill the buffer
		res = read(file->fd, file->r_buf, buf_size); // number of bytes read, 0:EOF, -1:error
		file->read_return_size = res;
		if (res == -1 || res == 0)  // error or EOF
			return -1;

		file->r_pos = -1; // reset read position
	}

	file->r_pos += 1; // increase read position by 1
	if (file->r_pos == file->read_return_size) { // reach EOF
		return -1;
	}
	char c = file->r_buf[file->r_pos]; // get character

	return (int)c; 
}



// Returns: character put if OK, -1 on error 
int fputc(int character, FILE * file) {
	if (file->w_buf == NULL) // first time do fputc, no write buffer yet 
		file->w_buf = (char *)malloc(file->buf_size);

	// put one character in write buffer then check if buffer is full
	char c = (char)character;
	file->w_pos += 1;
	file->w_buf[file->w_pos] = c;

	int res;
	int buf_size = file->buf_size;

	if (file->w_pos == buf_size - 1) { // buffer is full
		// dump the buffer content into the file
		res = write(file->fd, file->w_buf, buf_size); // number of bytes written, -1: error
		if (res == -1) { // error when writing to file
			file->w_pos -= 1; // reset write position to previous position
			return -1;
		}
		else if (res < buf_size) { // not all data in the buffer written to the file
			memmove(file->w_buf, file->w_buf + res, buf_size - res);
			file->w_pos = buf_size - res - 1;
		}
		else // all data in writing buffer have written to file
			file->w_pos = -1; // reset write position
	}
	
	return character;	

}



// Returns: character if OK, -1 on error
int ungetc(int character, FILE * file) {
	file->r_pos -= 1;
	int buf_size = file->buf_size;
	if (file->r_pos < -1) {
		int curroffset = lseek(file->fd,0,SEEK_CUR);
		if (curroffset <= buf_size) {
			return -1; // no more character to be unget
		}
		else if (curroffset < 2 * buf_size) {
			lseek(file->fd, 0, SEEK_SET);
			read(file->fd, file->r_buf, curroffset - buf_size);
			file->r_pos = curroffset - buf_size - 2;
		} 
		else {	
			lseek(file->fd, - 2 * buf_size, SEEK_CUR);
			read(file->fd, file->r_buf, buf_size);
			file->r_pos = buf_size - 2;
		}
	}
	file->r_buf[file->r_pos + 1] = (char)character;

	return character;
}



// Flush unwritten data to the file, clean write buffer
// Returns: 0 if OK, -1 on error
int fflush(FILE * file) {
	int res = 0; // OK when flush without writing buffer set up
	if (file->w_buf != NULL) {
		int pos = file->w_pos;
		res = write(file->fd, file->w_buf, file->w_pos + 1);
		if (res == -1) return -1; // error
		else if (res < file->w_pos + 1) { // not all flushed
				memmove(file->w_buf, file->w_buf + res, file->w_pos - res + 1);
				file->w_pos = file->w_pos - res;
				return -1;
		}
		else { //res == file->w_pos + 1; all flushed
			memset(file->w_buf, 0, file->buf_size);
			file->w_pos = -1;
			res = 0; // flush OK 
		}
	}
	return res;
}
	
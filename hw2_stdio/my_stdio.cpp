# include "my_stdio.h"
# include <fcnl.h>
# include <unistd.h>


FILE ** fopen(int pathname_count, char ** pathname_arr, int buf_size) {
	FILE ** file_arr;
	int i;
	for (i = 0; i < pathname_count, ++i) {	
		FILE * file;
		file->fd = open(pathname_arr[i], O_RDWR|O_CREAT, 0644); // 0 indicateds octal notation 
		file->buf_size = buf_size;
		file->read_return_size = buf_size;
		file->r_buf = NULL;
		file->w_buf = NULL;
		file->r_pos = -1;
		file->w_pos = -1;
		file_arr[i] = file;
	}
	return file_arr;
}	

int fclose(FILE * file) {
	int fd = file->fd;
	int res;
	free(file->r_buf);
	res = fflush(file); // returns 0 if OK, -1 on error
	if (res < 0) {
		printf("flush failed");
		return res;
	}
	free(file->w_buf);
	res = close(fd); // returns 0 if OK, -1 on error
	return res;
}

int fgetc(FILE * file) {
	int res;
	int buf_size = file->buf_size;

	if (file->r_buf == NULL || file->r_pos == buf_size -1) {
		if (file->r_buf == NULL)
			file->r_buf = (char *)malloc(buf_size);

		// fill the buffer
		res = read(file->fd, file->r_buf, buf_size); // number of bytes read, 0:EOF, -1:error
		file->read_return_size = res;
		if (res < 0)  // error 
			return res;

		file->r_pos = -1; // reset read position
	}

	file->r_pos += 1; // increase read position by 1
	if (file->r_pos == read_return_size) { // reach EOF
		return EOF;
	}
	char c = file->r_buf[file->r_pos]; // get character from the current read position

	return c; 
}

int fputc(FILE * file, int character) {
	if (file->w_buf == NULL) // first time do fputc, no write buffer yet 
		file->w_buf = (char *)malloc(file->buf_size);

	// put one character in write buffer then check if buffer is full
	char c = (char)character;
	file->w_pos += 1;
	file->w_buf[file->w_pos] = c;

	int res;
	int buf_size = file->buf_size;

	if (w_pos == buf_size - 1) { // buffer is full
		// dump the buffer content into the file
		res = write(file->fd, file->w_buf, buf_size); // number of bytes written, -1: error
		if (res < 0 || res < buf_size) { // error when writing to file
			file->w_pos -= 1; // reset write position to previous position
			return res;
		}
		file->w_pos = -1; // reset write position
	}
	
	else 
		return character;	

}

void ungetc(FILE * file) {
	file->r_pos -= 1;
	// TODO`
	// lseek current 0
	// or offset in struct
}

int fflush(FILE * file) {
	if (file->w_buf != NULL) {
		write(file->fd, file->w_buf, file->w_pos+1);i
		memset(w_buf, 0, file->buf_size);
	}
	return file->w_pos + 1;
}
	
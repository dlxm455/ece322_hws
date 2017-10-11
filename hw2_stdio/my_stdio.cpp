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
	free(file->w_buf);
	res += close(fd); // returns 0 if OK, -1 on error
	return res;  // returns 0 if OK, -1 if fflush or close failed; -2 if both fail;
}

int fgetc(FILE * file) {
	int r_pos = file->r_pos;
	int res; // TODO: eof
		`
	if (file->r_buf == NULL || r_pos == file->buf_size -1) {
		if (file->r_buf == NULL)	
			file->r_buf = (char*)malloc(file->buf_size);
		res = read(file->fd, file->r_buf, sizeof(int);
		if (res == 0) {
			file->r_pos = 0;
			return (int)file->r_buf[0];
		}
		else {
			return EOF;
		}
	}

	file->r_pos += 1;
	return file->r_buf[file->r_pos];

}

void fputc(FILE * file, int character) {
	if (file->w_buf == NULL) 
		file->w_buf = (char *)malloc(file->buf_size);
	char c = (char)character;
	int res;
	file->w_pos += 1;
	w_pos = file->w_pos;
	file->w_buf[w_pos] = c;	
	if (w_pos == file->buf_size-1) {
		res = write(file->fd, file->w_buf, file->buf_size);
		if (res == -1) {
			file->w_pos -= 1;
			return -1;
		}
	}
	else 
		return character;	

}

void ungetc(FILE * file) {
	file->r_pos -= 1;
}

int fflush(FILE * file) {
	if (file->w_buf != NULL) {
		write(file->fd, file->w_buf, file->w_pos+1);i
		memset(w_buf, 0, file->buf_size);
	}
	return file->w_pos + 1;
}
	
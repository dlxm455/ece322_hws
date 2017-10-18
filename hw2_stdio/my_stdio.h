# ifndef _MY_STDIO_H_
# define _MY_STDIO_H_

typedef struct FILE_struct {
	int fd;
	int buf_size;
	int read_return_size;
	char * r_buf;
	char * w_buf;
	int r_pos;
	int w_pos;
	//int cur_offset;
	
} FILE;

//FILE ** fopen(char **, int, int);
FILE * fopen(char *, char *, int);
int fclose(FILE *);
int fgetc(FILE *);
int fputc(int, FILE *);
int ungetc(int, FILE *);
int fflush(FILE *);

#endif

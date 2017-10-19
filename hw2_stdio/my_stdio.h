# ifndef _MY_STDIO_H_
# define _MY_STDIO_H_

typedef struct FILE_struct {
	int fd;
	int buf_size;
	int read_return_size; // use to detect EOF when reading 
	char * r_buf;
	char * w_buf;
	int r_pos; // previous read position in read buffer
	int w_pos; // previous write position in write buffer
	
} FILE;

//FILE ** fopen(char **, int, int);
FILE * fopen(char *, char *, int);
int fclose(FILE *);
int fgetc(FILE *);
int fputc(int, FILE *);
int ungetc(int, FILE *);
int fflush(FILE *);

#endif

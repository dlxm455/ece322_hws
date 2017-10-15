# ifndef _MY_STDIO_H_
# define _MY_STDIO_H_

struct FILE {
	int fd;
	int buf_size;
	int read_return_size;
	char * r_buf;
	char * w_buf;
	int r_pos;
	int w_pos;
	
};

FILE * fopen(char *);
int fclose(FILE *);
int fgetc(FILE *);
int fputc(FILE *, int);
void ungetc(FILE *);
int fflush(FILE *);



#endif
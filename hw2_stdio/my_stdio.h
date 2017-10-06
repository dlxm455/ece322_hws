# ifndef _MY_STDIO_H_
# define _MY_STDIO_H_

struct FILE {
	int fd;
	int buf_size;
	int * r_buf;
	int * w_buf;
	
};

FILE * fopen(char *);
int fclose(FILE *);
int fgetc(FILE *);
int fputc(FILE *);
int ungetc(FILE *);
int fflush(FILE *);



#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void find_progname(char * progname, char * pathname) {
	int ind = strlen(pathname) - 1;
	while (pathname[ind] != '/') {
		ind--;
	}
	strcpy(progname, pathname+ind);
} 
		
int main() {
	
	char cat_fn[128];
	char sort_fn[128];
	char merge_fn[128];
	char format_fn[128];
	char data_fn[128];
	int num_sort;


	fprintf(stderr, "%s", "The pathname of the splitting program:\n");
	if (scanf("%s", cat_fn) != 1) {
		fprintf(stderr, "read fails.");
		exit(1);
	}

	fprintf(stderr, "%s", "The pathname of the sorting program:\n");
	if (scanf("%s", sort_fn) != 1) {
		fprintf(stderr, "read fails.");
		exit(1);
	}

	fprintf(stderr, "%s", "The pathname of the merging program:\n");
	if (scanf("%s", merge_fn) != 1) {
		fprintf(stderr, "read fails.");
		exit(1);
	}

	fprintf(stderr, "%s", "The pathname of the formating program:\n");
	if (scanf("%s", format_fn) != 1) {
		fprintf(stderr, "read fails.");
		exit(1);
	}

	fprintf(stderr, "%s", "The pathname of the data file with the source data:\n" );
	if (scanf("%s", data_fn) != 1) {
		fprintf(stderr, "read fails.");
		exit(1);
	}

	fprintf(stderr, "%s", "The number of string to sort: ");
	if (scanf("%d", &num_sort) !=1) {
		fprintf(stderr, "read fails.");
		exit(1);
	}
	
	
	// Create 5 pipes
	int fd1[2], fd2[2], fd3[2], fd4[2], fd5[2];

	// create 5 pipes
	pipe(fd1);
	pipe(fd2);
	pipe(fd3);
	pipe(fd4);
	pipe(fd5);

	// create 7 children from back to front in the pipeline
	// create myMerge2
	pid_t pid1 = fork();
	if (pid1 < 0) { // fail check
		fprintf(stderr, "fork myMerge2 failed\n");
		exit(1);
	}
	else if (pid1 == 0) {
		close(fd5[1]); // close pipe5 write end
		dup2(fd5[0], STDIN_FILENO); // connect pipe5 read end to stdin

		// close other pipes
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		close(fd3[0]);
		close(fd3[1]);
		close(fd4[0]);
		close(fd4[1]);


		// execute myMerge2
		char merge_prn[64];
		find_progname(merge_prn, merge_fn);
		char num1_buf[64];
		sprintf(num1_buf, "%d", num_sort / 2);
        char num2_buf[64];
        sprintf(num2_buf, "%d", num_sort - num_sort / 2);

		execl(merge_fn, merge_prn, num1_buf, num2_buf, (char*)0);


		close(fd5[0]); // close pipe5 read end

		exit(0);	
	}

	// create first myFormat2
	pid_t pid2 = fork();
	if (pid2 < 0) {
		fprintf(stderr, "fork 1st myFormat2 failed\n");
		exit(1);
	}
	else if (pid2 == 0) {
		close(fd5[0]); // close pipe5 read end
		dup2(fd5[1], STDOUT_FILENO); // connect pipe5 write end to stdout

		close(fd3[1]); // close pipe3 write end
		dup2(fd3[0], STDIN_FILENO); // connect pipe3 read end to stdin

		// close other pipes
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		close(fd4[0]);
		close(fd4[1]);


		// execute 1st myFormat2
		char format_prn[64];
		find_progname(format_prn, format_fn);

		execl(format_fn, format_prn, "1", (char*) 0);	


		close(fd5[1]); // close pipe5 write end
		close(fd3[0]); // close pipe3 read end

		exit(0);
	}

	// create second myFormat2
	pid_t pid3 = fork();
	if (pid3 < 0) {
		fprintf(stderr, "fork 2nd myFormat2 failed\n");
		exit(1);
	}
	else if (pid3 == 0) {
		close(fd5[0]); // close pipe5 read end
		dup2(fd5[1], STDOUT_FILENO); // connect pipe5 write end to stdout

		close(fd4[1]); // close pipe4 write end
		dup2(fd4[0], STDIN_FILENO); // connect pipe4 read end to stdin

		// close other pipes
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		close(fd3[0]);
		close(fd3[1]);


		// execute 2nd myFormat2
		char format_prn[64];
		find_progname(format_prn, format_fn);

		execl(format_fn, format_prn, "2", (char*) 0);	



		close(fd5[1]); // close pipe5 write end
		close(fd4[0]); // close pipe4 read end

		exit(0);
	}

	// create first mySort2
	pid_t pid4 = fork();
	if (pid4 < 0) {
		fprintf(stderr, "fork 1st mySort2 failed\n");
		exit(1);
	}
	else if (pid4 == 0) {
		close(fd3[0]); // close pipe3 read end
		dup2(fd3[1], STDOUT_FILENO); // connect pipe3 write end to stdout

		close(fd1[1]); // close pipe1 write end
		dup2(fd1[0], STDIN_FILENO); // connect pipe1 read end to stdin

		// close other pipes
		close(fd2[0]);
		close(fd2[1]);
		close(fd4[0]);
		close(fd4[1]);
		close(fd5[0]);
		close(fd5[1]);


		// execute mySort2
		char sort_prn[64]; // program name
		find_progname(sort_prn, sort_fn);

		execl(sort_fn, sort_prn, (char *)0); 




		close(fd3[1]); // close pipe3 write end
		close(fd1[0]); // close pipe1 read end

		exit(0);
	}

	// create second mySort2
	pid_t pid5 = fork();
	if (pid5 < 0) {
		fprintf(stderr, "fork 2nd mySort2 failed\n");
		exit(1);
	}
	else if (pid5 == 0) {
		close(fd4[0]); // close pipe4 read end
		dup2(fd4[1], STDOUT_FILENO); // connect pipe4 write end to stdout

		close(fd2[1]); // close pipe2 write end
		dup2(fd2[0], STDIN_FILENO); // connect pipe2 read end to stdin

		// close other pipes
		close(fd1[0]);
		close(fd1[1]);
		close(fd3[0]);
		close(fd3[1]);
		close(fd5[0]);
		close(fd5[1]);


		// execute mySort2
		char sort_prn[64]; // program name
		find_progname(sort_prn, sort_fn);

		execl(sort_fn, sort_prn, (char *)0); 



		close(fd4[1]);
		close(fd2[0]);

		exit(0);
	}

	// create first myCat2
	pid_t pid6 = fork();
	if (pid6 < 0) {
		fprintf(stderr, "fork 1st myCat2 failed\n");
		exit(1);
	}
	else if (pid6 == 0) {
		close(fd1[0]); // close pipe1 read end
		dup2(fd1[1], STDOUT_FILENO); // connect pipe1 write end to stdout

		// close other pipes
		close(fd2[0]);
		close(fd2[1]);
		close(fd3[0]);
		close(fd3[1]);
		close(fd4[0]);
		close(fd4[1]);
		close(fd5[0]);
		close(fd5[1]);


		// execute 1st myCat2
		char cat_prn[64]; // program name
		find_progname(cat_prn, cat_fn);
		char num_buf1[64];
		char num_buf2[64];
		sprintf(num_buf1, "%d", 1);
		sprintf(num_buf2, "%d", num_sort / 2);
		
		execl(cat_fn, cat_prn, data_fn, num_buf1, num_buf2, (char *)0); 


		close(fd1[1]);

		exit(0);
	}

	// create second myCat2
	pid_t pid7 = fork();
	if (pid7 < 0) {
		fprintf(stderr, "fork 1st myCat2 failed\n");
		exit(1);
	}
	else if (pid7 == 0) {
		close(fd2[0]); // close pipe2 read end
		dup2(fd2[1], STDOUT_FILENO); // connect pipe2 write end to stdout

		// close other pipes
		close(fd1[0]);
		close(fd1[1]);
		close(fd3[0]);
		close(fd3[1]);
		close(fd4[0]);
		close(fd4[1]);
		close(fd5[0]);
		close(fd5[1]);


		// execute 2nd myCat2
		char cat_prn[64]; // program name
		find_progname(cat_prn, cat_fn);
		char num_buf1[64];
		char num_buf2[64];
		sprintf(num_buf1, "%d", num_sort / 2 + 1);
		sprintf(num_buf2, "%d", num_sort);
		
		execl(cat_fn, cat_prn, data_fn, num_buf1, num_buf2, (char *)0); 



		close(fd2[1]);

		exit(0);
	}

	// close all pipe ends
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
	close(fd3[0]);
	close(fd3[1]);
	close(fd4[0]);
	close(fd4[1]);
	close(fd5[0]);
	close(fd5[1]);

	// waits for children to complete
	int ccount = 7;
	pid_t pidw;

	while(ccount--) {
		pidw = wait(NULL);
	}

	exit(0);
}



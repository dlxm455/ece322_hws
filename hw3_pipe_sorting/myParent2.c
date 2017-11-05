#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void parent_behavior(int * fd, char * num_str) {
		int num = atoi(num_str);
		close(fd[0]); // close read end of the pipe

		FILE *fp = fdopen(fd[1], "w"); 

		char str[128];
		int count = 0;
		while((count < num) && (scanf("%s", str) != EOF)) {
			str[127] = '\0'; // for safe
			fprintf(fp, "%s\n", str); // write to pipe 
			fflush(fp); // flush to pipe
			count++;
		}
		fclose(fp); // close write end
}

void child_behavior(int *fd, char * argv[], char suffix) {

		close(fd[1]); // close write end of the pipe
		
		dup2(fd[0], STDIN_FILENO); // connect read end to stdin 

		// find the name for the program from the pathname
		char * pathname = argv[1];
		int pathname_len = strlen(pathname);
		int ind = pathname_len-1;
		while (pathname[ind] != '/') {
			ind--;
		}

		// malloc space for name including '\0'
		char * prog_name = (char *)malloc(pathname_len - ind);
		strcpy(prog_name, pathname+ind); 
		
		int argfname_len = strlen(argv[3]);
		char * out_fname = (char *)malloc(argfname_len + 3); // add '_', suffix, '\0'
		strcpy(out_fname, argv[3]);
		out_fname[argfname_len] = '_';
		out_fname[argfname_len + 1] = suffix;
		out_fname[argfname_len + 2] = '\0';

		// prepare parameter vector
		char * paramVec[4] = {prog_name, argv[2], out_fname, NULL};

		// execute mySort 
		execv(argv[1], paramVec);

		close(fd[0]); 

		exit(0);

}

int main(int argc, char * argv[]) {

	int fd1[2];
	pid_t pid1;

	// create 1st pipe
	if (pipe(fd1) < 0) {
		fprintf(stderr, "pipe1 failed\n");
		exit(1);
	}

	// create 1st child
	pid1 = fork();

	if (pid1 < 0) { 
		fprintf(stderr, "fork1 failed\n");
		exit(1);
	}

	else if (pid1 > 0) { // parent
		int fd2[2];
		pid_t pid2;

		// create 2nd pipe
		if (pipe(fd2) < 0) {
			fprintf(stderr, "pipe1 failed\n");
			exit(1);
		}
		
		pid2 = fork();

		if (pid2 < 0) {
			fprintf(stderr, "fork2 failed\n");
			exit(1);
		}

		else if (pid2 > 0) { // parent
			parent_behavior(fd1, argv[2]);
			parent_behavior(fd2, argv[2]);
			
			// wait for both children to complete
			int ccount = 2;
			pid_t pidw;

			while (ccount--) {
				pidw = wait(NULL);
			}
			
				
		}

		else { // child2
			child_behavior(fd2, argv, '2');
		}
		
	}

	else { // child1
		child_behavior(fd1, argv, '1');
	}

	exit(0);
	
}


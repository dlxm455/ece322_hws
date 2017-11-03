#include <stdio.h>

void parent_behavior(int * fd, char * argv[]) {

		close(fd[0]);

		FILE *fp = fdopen(fd[1], "w");

		char *str[128];
		int count = 0;
		while(count < argv[2] && scanf("%s", str) != EOF) {
			str[127] = '\0';
			fprintf(fp, "%s", str);
			fflush(fp);
		}
		fclose(fp);
}

void child_behavior(int *fd, char * argv[], char * out_fname) {

		close(fd[1]);
		
		dup2(fd[0], STDIN_FILENO);

		execv(argv[1], ["my_Sort", argv[2], out_fname, NULL]);

		close(fd[0]);
		exit(0);

}

int main(int argc, char * argv[]) {
	// first pipe
	int fd[2], fd2[2];
	pid_t pid, pid2, pidw;
	int ccount = 2;

	// create 1st pipe
	if (pipe(fd) < 0) {
		fprintf(stderr, "pipe1 failed\n");
		exit(1);
	}

	// create 1st child
	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork1 failed\n");
		exit(1);
	}

	else if (pid > 0) { // parent
		parent_behavior(fd);
	}

	else { // child1
		child_behavior(fd, argv[], "sout.txt_1");
	}



	// create 2nd pipe 
	if (pipe(fd2) < 0) {
		fprintf(stderr, "pipe2 failed\n");
		exit(1);
	}
	
	// create 2nd child
	pid2 = fork();
	
	if (pid2 < 0) {
		fprintf(stderr, "fork2 failed\n");
		exit(1);
	}

	else if (pid2 > 0) { // parent
		parent_behavior(fd2, argv[]);
	}

	else { // child2
		child_behavior(fd2, argv[], "sout.txt_2");	
	}

	while (ccout--) {
		pidw = wait(NULL);
		printf("parent: child with pid of %d completed\n", pidw);
	}

	exit(0);
	
}


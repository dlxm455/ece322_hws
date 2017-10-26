#include <stdio.h>


int main(int argc, char * argv[]) {
	
	int fd[2];
	
	if (pipe(fd) < 0) {
		fprintf(stderr, "pipe failed\n");
		exit(1);
	}

	pid_t pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}

	else if (pid > 0) { // parent
		close(fd[0]); // close read end
		
		FILE *fp = fdopen(fd[1],"w");

		char str[128]; // buffer to store scanf result
		int count = 0;
		while (count < argv[2], scanf("%s", str) != EOF) {
			str[127] = '/0'; // for safe
			fprintf(fp, "%s", str); // write to FIFO pointed by fd[1]
			fflush(fp); // flush to FIFO 
			count++;
		}

		fclose(fp); // close write end
		pid_t p = wait(NULL);
		printf("parent: child with pid of %d completed\n", p); 
	}

	else { // child
		close(fd[1]); // close write end

		dup2(fd[0], STDIN_FILENO); // connect to stdin
		
		char *const paraList[argc]; //name, additional param (argc-2), NULL
		for (int i = 0; i < num_para; i++) {
			if (i == 0) paraList[i] = "mySort";
			if (i == num_para - 1) paraList[i] = NULL;
			else paraList[i] = argv[i+2];
		}
		execv(argv[1], num_para);

		close(fd[0]);
		
		exit(0);
	}

	exit(0);
}


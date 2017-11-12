#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char * argv[]) {
	
	int fd[2];
	
	if (pipe(fd) < 0) { // create a pipe
		fprintf(stderr, "pipe failed\n");
		exit(1);
	}

	pid_t pid = fork(); // create a child

	if (pid < 0) {
		fprintf(stderr, "fork failed\n");
		exit(1);
	}

	else if (pid > 0) { // parent

		close(fd[0]); // close read end
		
		FILE *fp = fdopen(fd[1],"w");

		// Read the data file
		char str[128]; // buffer to store scanf result
		int count = 0;
		int arg_num = atoi(argv[2]);
		while ((count < arg_num) && (scanf("%s", str) != EOF)) {
			str[127] = '\0'; // for safe
			fprintf(fp, "%s\n", str); // write to pipe
			fflush(fp); // flush to pipe 
			count++;
		}

		fclose(fp); // close write end
        
        //fprintf(stdout, "child pid: %d\n", pid);
		pid_t p = wait(NULL); // wait for child
        
       // fprintf(stdout, "wait pid: %d\n", p);
        exit(0);

	}

	else { // child
		close(fd[1]); // close write end of pipe

		dup2(fd[0], STDIN_FILENO); // connect read end of pipe to stdin
		
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

		// prepare parameter vector
		int num_param = argc; // argc - 2 + 2
		char *paramVec[num_param]; //name, additional param (argc-2), NULL
		int i;
		for (i = 0; i < num_param; i++) {
			if (i == 0) paramVec[i] = prog_name; 
			else if (i == num_param - 1) paramVec[i] = NULL;
			else paramVec[i] = argv[i+1];
		}

		// exec program with arguments as a vector
		execv(pathname, paramVec);

		close(fd[0]);
	
		exit(0);
	}

	//exit(0);
}



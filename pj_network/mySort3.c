#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //
#include <arpa/inet.h> //
#include <netdb.h> //
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

extern int errno; //?

// comparison function for qsort()
int cmpfunc(const void *a, const void *b) {
	return strcmp(*(char **)a, *(char **)b);
}

// function to free memories
void free_memories(int num, char ** data) {
	int i;
	for (i = 0; i < num; i++) {
		if (data[i]) free(data[i]);
	}
	if (data) free(data);
}

int main(int argc, char * argv[]) {

	struct servent *pse;
	struct sockaddr_in sin;
	struct sockaddr_in fsin;

	int s;
	int ssock;

	unsigned int fsin_size;

	int sort_num;
	int cc;
	int count;
	char ** data;
	char buf[128];
	int len;
	FILE * ssock_FILE;

	// set up socket address
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;	
	sin.sin_port = htons((unsigned short)atoi(argv[1]));

	// create the socket
	s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		printf("Cannot get socket\n");
		exit(1);
	}

	// bind to local address
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		printf("Cannot bind\n");
		exit(1);
	}

	// make socket passive
	if (listen(s,2) < 0) {
		printf("Cannot listen\n");
		exit(1);
	}

	while(1) {
		printf("Listening on %s...\n", argv[1]);
		fsin_size = sizeof(fsin);

		// wait for client to connect
		ssock = accept(s, (struct sockaddr *)&fsin, &fsin_size);

		if (ssock < 0) {
			printf("Cannot accept\n");
			exit(1);
		}

		// === Application Protocol ===

		// read in number from socket
        if (recv(ssock, &sort_num, sizeof(sort_num), 0) < 0) {
			printf("Error on read number\n");
			exit(1);
		}

		// allocate space for array of char* in heap
		data = (char **)malloc(sizeof(char *) * sort_num);

		// read data from socket to array
		count = 0;
		memset(buf, 0, sizeof(buf));
		while ((count < sort_num) && (recv(ssock, buf, sizeof(buf), 0) >= 0)) { // ?=0
			len = strlen(buf);
			data[count] = (char *)malloc(len+1);
			strcpy(data[count], buf);
			data[count][len] = '\0';
			count++;
			memset(buf, 0, sizeof(buf));
		}
		if (count != sort_num) {
			printf("not all data read in array\n");
			free_memories(sort_num, data);
			exit(1);
		}

		// quick sort
		qsort(data, sort_num, sizeof(char*), cmpfunc);

		ssock_FILE = fdopen(ssock, "w+");
		// write sorted data from array to socket
		for (count = 0; count < sort_num; count++) {
			//cc = send(ssock, data[count], strlen(data[count]) + 1, 0);
			cc = fprintf(ssock_FILE, "%s\n", data[count]);
			fflush(ssock_FILE);
			if (cc < 0) {
				printf("Error on write to socket\n");
				free_memories(sort_num, data);
				exit(1);
			}
		}

		// close socket
		close(ssock);
		
		// free memories 
		free_memories(sort_num, data);

	}

	printf("Sorting service is closing\n");
	fclose(ssock_FILE);
	close(s);
	return 0;
}
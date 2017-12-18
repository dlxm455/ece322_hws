#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

extern int errno;

// function to free memories
void free_memories(int num, char ** data) {
	int i;
	for (i = 0; i < num; i++) {
		if (data[i]) free(data[i]);
	}
	if (data) free(data);
}

int readToArr(char ** data, int arr_size, int sock) {
	char buf[128];
	int count = 0;
	int len;

	while (count < arr_size && recv(sock, buf, sizeof(buf), 0) >= 0) {
		len = strlen(buf);
		data[count] = (char *)malloc(len+1);
		strcpy(data[count], buf);
		data[count][len] = '\0';
		count++;
		memset(buf, 0, sizeof(buf));
	}
	return count;
}

int main(int argc, char * argv[]) {

	struct sockaddr_in sin;
	struct sockaddr_in fsin;

	int s;
	int ssock;

	unsigned int fsin_size;

	int num1, num2;
	int count1, count2;
	char ** data1; 
	char ** data2;

	int cc;
	int send_err_flag = 0;

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

		// read in num1 from socket
		if (recv(ssock, &num1, sizeof(num1), 0) < 0) {
			printf("Error on read num1\n");
			exit(1);
		}
		else printf("num1: %d\n", num1);

		// read in num2 from socket
		if (recv(ssock, &num2, sizeof(num2), 0) < 0) {
				printf("Error on read num2\n");
				exit(1);
		}
		else printf("num2: %d\n", num2);

		// allocate spacses for array1 and array2
		data1 = (char **)malloc(sizeof(char *) * num1);
		data2 = (char **)malloc(sizeof(char *) * num2); 

		// read data from socket to array1
		count1 = readToArr(data1, num1, ssock);
		if (count1 != num1) {
			printf("not all data read in array1\n");
			free_memories(num1, data1);
			free_memories(num2, data2);
			exit(1);
		}

		// read data from socket to array2
		count2 = readToArr(data2, num2, ssock);
		if (count2 != num2) {
			printf("not all data read in array2\n");
			free_memories(num1, data1);
			free_memories(num2, data2);
			exit(1);
		}

		// send to socket while merging
		FILE * ssock_FILE = fdopen(ssock, "w+");
		count1 = 0;
		int i, j;
		while (count1 < num1 || count2 < num2) {
			if (count1 == num1) {
					while(count2 < num2) {
						//cc = send(ssock, data2[count2], strlen(data2[count2]) + 1, 0);
						cc = fprintf(ssock_FILE, "%s\n", data2[count2]);
						fflush(ssock_FILE);
						if (cc < 0) {
							send_err_flag = 1;
							break;
						}
						count2++;
					}
			}
			else if (count2 == num2) {
				while(count1 < num1) {
					//cc = send(ssock, data1[count1], strlen(data1[count1]) + 1, 0);
					cc = fprintf(ssock_FILE, "%s\n", data1[count1]);
					fflush(ssock_FILE);
					if (cc < 0) {
						send_err_flag = 1;
						break;
					}
					count1++;
				}
			}
			int cmpRes = strcmp(data1[count1], data2[count2]);
			if (cmpRes < 0) {
				//cc = send(ssock, data1[count1], strlen(data1[count1]) + 1, 0);
				cc = fprintf(ssock_FILE, "%s\n", data1[count1]);
				if (cc < 0) {
					send_err_flag = 1;
					break;
				}
				count1++;
			}
			else {
				//cc = send(ssock, data2[count2], strlen(data2[count2]) + 1, 0);
				cc = fprintf(ssock_FILE, "%s\n", data2[count2]);
				if (cc < 0) {
					send_err_flag = 1;
					break;
				}
				count2++;
			} 
		}

		// close socket
		fclose(ssock_FILE);
		close(ssock);

		// free memories
		free_memories(num1, data1);
		free_memories(num2, data2);

		if (send_err_flag == 1) {
			printf("Error on write to socket\n");
			exit(1);
		}
	}

	printf ("Merge service is closing\n");
	close(s);
	return 0;
}
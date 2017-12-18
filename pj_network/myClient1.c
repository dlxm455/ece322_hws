#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/errno.h>

int main(int argc, char * argv[]) {
	char hn1[], hn2[], hn3[];
	int p1, p2, p3;
	int num_str;
	int c_level = 2;
	int num_each;

// Configure: (from STDIN)
	printf("Please enter hostname and port of mySort Service 1:\n");
	while (scanf("%s %d", hn1, &p1) != 2) {
		printf("Format: hostname port");
	}

	printf("Please enter hostname and port of mySort Service 2:\n");
	while (scanf("%s %d", hn2, &p2) != 2) {
		printf("Format: hostname port");
	}

	printf("Please enter hostname and port of myMerge:\n");
	while (scanf("%s %d", hn3, &p3) != 2) {
		printf("Format: hostname port");
	}

	printf("Please enter number of strings:\n");
	while (scanf("%d", &num_str) != 1) {
		printf("Format: number");
	}

	printf("C-level for this version is 2\n");

	num_each = num_str / c_level;

	// create three sockets
	int s1, s2, s3;

	s1 = socket(PF_INET, SOCK_STREAM, 0);
	s2 = socket(PF_INET, SOCK_STREAM, 0);
	s3 = socket(PF_INET, SOCK_STREAM, 0);

	// create three socket addresses
	struct sockaddr_in sin1, sin2, sin3;

	memset(&sin1, 0, sizeof(sin1));
	memset(&sin2, 0, sizeof(sin2));
	memset(&sin3, 0, sizeof(sin3));
	sin1.sin_family = AF_INET;
	sin2.sin_family = AF_INET;
	sin3.sin_family = AF_INET;

	// configures socket addresses
	struct hostent *phe;
	if (phe = gethostbyname(hn1)) {
		memcpy(&sin1.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		printf("Cannot get host %s\n", hn1);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	sin1.sin_port = htons((unsigned short)p1);

	if (phe = gethostbyname(hn2)) {
		memcpy(&sin2.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		printf("Cannot get host %s\n", hn2);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	sin2.sin_port = htons((unsigned short)p2);

	if (phe = gethostbyname(hn3)) {
		memcpy(&sin3.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		printf("Cannot get host %s\n", hn3);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	sin3.sin_port = htons((unsigned short)p3);

// Connect/send/recv data: mySort Service 1
	if (connect(s1, (struct sockaddr *)&sin1, sizeof(sin1)) < 0) {
		printf("Cannot connect s1\n");
		exit(1);
	}

	// send number of strings 
	if (send(s1, &num_each, sizeof(int), 0) < 0){
		printf("Sent number: %d to mySort service 1\n", num_each);
		exit(1);
	}

	int count = 0;
	char str[128];
	// send strings to sort
	FILE * s1_FILE = fdopen(s1, "w");
	while ((count < num_each) && (scanf("%s", str) == 1)) {
		fprintf(s1_FILE, "%s\n", str); // ??? need \n or not
		fflush(s1_FILE);
		count++;
	} 

	if (count != num_each) {
		printf("not all data sent to mySort service 1\n");
		exit(1);
	}

	// read sorted strings 
	FILE * outfile = fopen("network_sort_output", "w");
	count = 0;	
	while ((count < num_each) && (fscanf(s1_FILE, "%s", str) == 1)) {
		fprintf(outfile, "%s\n", str);
		count++;
	}
	
	if (count != num_each) {
		printf("not all data received from mySort service 1\n");
		exit(1);
	}

	fclose(outfile);
	fclose(s1_FILE);
	close(s1);


// Connect/send/recv data: mySort Service 2
	if (connect(s2, (struct sockaddr *)&sin2, sizeof(sin2)) < 0) {
		printf("Cannot connect s2\n");
		exit(1);
	}

	// send number of strings 
	if (send(s2, &num_each, sizeof(int), 0) < 0){
		printf("Sent number: %d to mySort service 2\n", num_each);
		exit(1);
	}

	count = 0;
	// send strings to sort
	FILE * s2_FILE = fdopen(s2, "w");
	while ((count < num_each) && (scanf("%s", str) == 1)) {
		fprintf(s2_FILE, "%s\n", str); // ??? need \n or not
		fflush(s2_FILE);
		count++;
	} 

	if (count != num_each) {
		printf("not all data sent to mySort service 2\n");
		exit(1);
	}

	// read sorted strings 
	FILE * outfile = fopen("network_sort_output", "a");
	count = 0;	
	while ((count < num_each) && (fscanf(s2_FILE, "%s", str) == 1)) {
		fprintf(outfile, "%s\n", str);
		count++;
	}
	
	if (count != num_each) {
		printf("not all data received from mySort service 2\n");
		exit(1);
	}

	fclose(outfile);
	fclose(s2_FILE);
	close(s2);


// Connect/send/recv data: myMerge Service
	if (connect(s3, (struct sockaddr *)&sin3, sizeof(sin3)) < 0) {
		printf("Cannot connect s3\n");
		exit(1);
	}

	// send number of strings 
	if (send(s3, &num_each, sizeof(int), 0) < 0){
		printf("Sent number: %d to myMerge\n", num_each);
		exit(1);
	}

	if (send(s3, &num_each, sizeof(int), 0) < 0){
		printf("Sent number: %d to myMerge\n", num_each);
		exit(1);
	}

	count = 0;

	// send data to merge 
	FILE * s3_FILE = fdopen(s3, "w");
	while ((count < num_str) && (scanf("%s", str) == 1)) {
		fprintf(s3_FILE, "%s\n", str); // ??? need \n or not
		fflush(s3_FILE);
		count++;
	} 

	if (count != num_str) {
		printf("not all data sent to myMerge service\n");
		exit(1);
	}

	// read sorted strings and write to STDOUT
	count = 0;	
	while ((count < num_str) && (fscanf(s3_FILE, "%s", str) == 1)) {
		printf("%s\n", str);
		count++;
	}
	
	if (count != num_str) {
		printf("not all data received from myMerge service\n");
		exit(1);
	}

	fclose(s3_FILE);
	close(s3);

}
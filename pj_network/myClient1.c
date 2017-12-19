#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	char hn1[81];
	char hn2[81];
	char hn3[81];
	int p1, p2, p3;
	int num_str;
	int c_level = 2;
	int num_each;

	int count = 0;
	char str[128];
	int cc;
	char ** data;
	int count_arr;
	int len;


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
	if ((phe = gethostbyname(hn1))) {
		memcpy(&sin1.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		printf("Cannot get host %s\n", hn1);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	sin1.sin_port = htons((unsigned short)p1);

	if ((phe = gethostbyname(hn2))) {
		memcpy(&sin2.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		printf("Cannot get host %s\n", hn2);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	sin2.sin_port = htons((unsigned short)p2);

	if ((phe = gethostbyname(hn3))) {
		memcpy(&sin3.sin_addr, phe->h_addr, phe->h_length);
	}
	else {
		printf("Cannot get host %s\n", hn3);
		printf("Error: %s\n", strerror(errno));
		exit(1);
	}
	sin3.sin_port = htons((unsigned short)p3);

	// allocate space for array to save received data
	data = (char **)malloc(sizeof(char *) * num_str);
	count_arr = 0;
// Connect/send/recv data: mySort Service 1
	if (connect(s1, (struct sockaddr *)&sin1, sizeof(sin1)) < 0) {
		printf("Cannot connect s1\n");
		exit(1);
	}

	// send number of strings 
	if (send(s1, &num_each, sizeof(int), 0) < 0) {
		printf("Cannot send number: %d to mySort service 1\n", num_each);
		exit(1);
	}
	printf("Send number: %d to mySort service 1\n", num_each);

	// send strings to sort
	FILE * s1_FILE = fdopen(s1, "w+");
	count = 0;
	while (count < num_each) {
		cc = scanf("%s", str);
		if (cc < 0) break;	
		fprintf(s1_FILE, "%s", str); // ??? need \n or not
		fflush(s1_FILE);
		printf("count: %d\n", count);
		count++;
	}

	if (count != num_each) {
		printf("not all data sent to mySort service 1\n");
		exit(1);
	}


	// read sorted strings 
	//FILE * outfile = fopen("network_sort_output", "w");

	count = 0;
	//fprintf(s1_FILE, "ready to receive sorted data\n");
	//fflush(s1_FILE);

	while (count < num_each) {
		cc = fscanf(s1_FILE, "%s", str);
		if (cc < 0) break;
		len = strlen(str);
		data[count_arr] = (char *)malloc(len+1);
		strcpy(data[count_arr], str);
		data[count_arr][len] = '\0';
		//fprintf(outfile, "%s\n", str);
		//printf("count: %d %s\n", count, str);
		count++;
		count_arr++;
	}
	printf("receive %d strings\n", count);

	if (count != num_each) {
		printf("not all data received from mySort service 1\n");
		exit(1);
	}
	else {
		printf("all data received from mySort service 1\n");
	}

	//fclose(outfile);
	fclose(s1_FILE);
	close(s1);


// Connect/send/recv data: mySort Service 2
	if (connect(s2, (struct sockaddr *)&sin2, sizeof(sin2)) < 0) {
		printf("Cannot connect s2\n");
		exit(1);
	}

	// send number of strings 
	if (send(s2, &num_each, sizeof(int), 0) < 0){
		printf("Cannot send number: %d to mySort service 2\n", num_each);
		exit(1);
	}
	printf("Send number: %d to mySort service 2\n", num_each);

	// send strings to sort
	FILE * s2_FILE = fdopen(s2, "w+");
	count = 0;
	while (count < num_each) {
		cc = scanf("%s", str);
		if (cc < 0) break;
		fprintf(s2_FILE, "%s", str); // ??? need \n or not
		fflush(s2_FILE);
		printf("count: %d\n", count);
		count++;
	}

	if (count != num_each) {
		printf("not all data sent to mySort service 2\n");
		exit(1);
	}

	// read sorted strings
	// outfile = fopen("network_sort_output", "a");
	count = 0;
	//fprintf(s2_FILE, "ready to receive sorted data\n");
	//fflush(s2_FILE);

	while (count < num_each) {
		cc = fscanf(s2_FILE, "%s", str);
		if (cc < 0) break;
		len = strlen(str);
		data[count_arr] = (char *)malloc(len+1);
		strcpy(data[count_arr], str);
		data[count_arr][len] = '\0';
		//fprintf(outfile, "%s\n", str);
		printf("count: %d %s\n", count, str);
		count++;
		count_arr++;
	}

	printf("receive %d strings\n", count);

	if (count != num_each) {
		printf("not all data received from mySort service 2\n");
		exit(1);
	}
	else {
		printf("all data received from mySort service 2\n");
	}

	//fclose(outfile);
	fclose(s2_FILE);
	close(s2);



// Connect/send/recv data: myMerge Service
	if (connect(s3, (struct sockaddr *)&sin3, sizeof(sin3)) < 0) {
		printf("Cannot connect s3\n");
		exit(1);
	}

	// send number of strings
	if (send(s3, &num_each, sizeof(int), 0) < 0){
		printf("Cannot send number: %d to myMerge\n", num_each);
		exit(1);
	}
	else {
		printf("Sent number: %d to myMerge\n", num_each);
	}
	if (send(s3, &num_each, sizeof(int), 0) < 0){
		printf("Cannot send number: %d to myMerge\n", num_each);
		exit(1);
	}
	else {
		printf("Sent number: %d to myMerge\n", num_each);
	}

	count = 0;

	// send data to merge
	FILE * s3_FILE = fdopen(s3, "w+");
	count = 0;
	while (count < num_str) {
		fprintf(s3_FILE, "%s\n", data[count]); // ??? need \n or not
		fflush(s3_FILE);
		count++;
	}

	if (count != num_str) {
		printf("not all data sent to myMerge service\n");
		exit(1);
	}
	else {
		printf("sent all data to myMerge service\n");
	}

	// read sorted strings and write to STDOUT
	printf("\nmerged data: \n");
	count = 0;	
	while (count < num_str) {
		cc = fscanf(s3_FILE, "%s", str);
		if (cc < 0) break;
		printf("%s\n", str);
		count++;
	}
	
	if (count != num_str) {
		printf("not all data received from myMerge service\n");
		exit(1);
	}

	fclose(s3_FILE);
	close(s3);

	// free array memories
	int i;
	for (i = 0; i < num_str; i++) {
		if (data[i]) free(data[i]);
	}
	if (data) free(data);

	return 0;
}

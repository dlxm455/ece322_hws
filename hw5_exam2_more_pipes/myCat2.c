#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
    
    // get start line and end line indice from arguments
	int start_line = strtol(argv[2], NULL, 10);
	int end_line = strtol(argv[3], NULL, 10);


	// open data file
	FILE * datafile = fopen(argv[1], "r");
	int line_count = 1;
	char str[128]; // buffer for fscanf
	
	// read until startline
	while (line_count < start_line) {
		fscanf(datafile, "%s", str);
		line_count++;
	}
    
	// write the number of lines
	printf("%d\n", end_line - start_line + 1);
	//fflush(stdout);
	
	// read and write from startline to endline
	while (line_count <= end_line) {
		fscanf(datafile, "%s", str);
		printf("%s\n", str);
		//fflush(stdout);
		line_count++;
	}
	
    // close data file
	fclose(datafile);
}
	

	 
	
	
	
#include <stdio.h>

int main(int argc, char * argv[]) {
	char * code = argv[1];
	char buf[128]; 
	while (scanf("%s", buf) == 1) {
		printf("%s %s\n", code, buf);
	}
	return 0;
}



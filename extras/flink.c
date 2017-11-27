#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]) {
	DIR * my_DIR;
	int dir_ind = 1;
	while (dir_ind < argc) { 
		my_DIR = opendir(argv[dir_ind]);

		if (my_DIR == NULL) {
			fprintf(stderr, "Cannot open directory: %s\n", argv[dir_ind]);
			exit(1);
		}

		struct dirent * my_dirent;
		char path_buf[512];
		struct stat my_stat;
		struct stat my_stat_f;
		while ((my_dirent = readdir(my_DIR)) != NULL) {
			sprintf(path_buf, "%s/%s", argv[dir_ind], my_dirent->d_name);
			lstat(path_buf, &my_stat);
			if (S_ISLNK(my_stat.st_mode)) {
				printf("%s -> ", path_buf);
				char buf_f[512];
				int len;
				if ( (len = readlink(path_buf, buf_f, sizeof(buf_f)-1)) != -1) {
						buf_f[len] = '\0';
						printf("%s ", buf_f);
					}
				/*
				char *ptr;
				ptr = realpath(path_buf, buf_f);
				printf("readpath return: %s\n", ptr);
				printf("readpath: %s\n", buf_f); 
				*/	
				stat(path_buf, &my_stat_f);

				mode_t m_f = my_stat_f.st_mode;
				if (S_ISREG(m_f)) 
					printf("(file: size %lld bytes)\n", my_stat_f.st_size);

				else if (S_ISDIR(m_f))
					printf("(directory)\n"); 
			}
		}
		closedir(my_DIR);
		dir_ind++;
	}
	return 0;
}
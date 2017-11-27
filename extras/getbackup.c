#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <string.h>

// copy file
int fcopy(char *srcfname, char *destfname) {
	// get srcfname stat
	struct stat my_stat;
	int ret_lstat = lstat(srcfname, &my_stat);
	if (ret_lstat != 0) {
		fprintf(stderr, "getting %s info failed, it may not exist\n", srcfname);
		return -1;
	}

	// check if srcfname is a file
	if (S_ISREG(my_stat.st_mode) == 0) {
		fprintf(stderr, "%s is not a file\n", srcfname);
		return -1;
	}

	// open files
	int fd_src = open(srcfname, O_RDONLY);
	int fd_dest = open(destfname, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	// read and write
	char buf[4096];
	int size_rd, size_wr;
	while ((size_rd = read(fd_src, buf, sizeof(buf))) > 0) {
		size_wr = write(fd_dest, buf, size_rd);
	}

	// close files
	close(fd_src);
	close(fd_dest);
	
	return 0;
}

int dcopy(char * srcdir, char * destdir) {
	// get srcdir info
	struct stat my_stat;
	int ret_lstat = lstat(srcdir, &my_stat);
	if (ret_lstat != 0) {
		fprintf(stderr, "getting %s info failed, it may not exist\n", srcdir);
		return -1;
	}

	// check if srcdir is a directory
	if (S_ISDIR(my_stat.st_mode) == 0) {
		fprintf(stderr, "%s is not a directory\n", srcdir);
		return -1;
	}

	// create destination directory
	ret_lstat = lstat(destdir, &my_stat);
	if (ret_lstat == 0) {
		fprintf(stderr, "%s already exists\n", destdir);
		return -1;
	}
	int ret_mkdir = mkdir(destdir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IROTH);	if (ret_mkdir != 0) {
		fprintf(stderr, "create %s failed\n", destdir);
		return -1;
	}

	// open srcdir 
	DIR * my_DIR;
	my_DIR = opendir(srcdir);
	if (my_DIR == NULL) {
		fprintf(stderr, "open %s failed\n", srcdir);
		return -1;
	}

	// copy files or directories
	struct dirent * my_dirent;
	char srcname[512];
	char destname[512];
	char * dname;
	while ((my_dirent = readdir(my_DIR)) != NULL) {
		dname = my_dirent->d_name;
		if (strcmp(dname, ".") != 0 && strcmp(dname, "..") != 0) {
			sprintf(srcname, "%s/%s", srcdir, dname);
			sprintf(destname, "%s/%s", destdir, dname);
			ret_lstat = lstat(srcname, &my_stat);
			if (S_ISREG(my_stat.st_mode))
				fcopy(srcname, destname);
			else if (S_ISDIR(my_stat.st_mode))
				dcopy(srcname, destname); // copy directory recursively
		}
	}

	closedir(my_DIR);

	return 0;
}

int dremove(char * dir) {
	DIR * my_DIR;
	my_DIR = opendir(dir);
	if (my_DIR == NULL) {
		fprintf(stderr, "open %s failed\n", dir);
		return -1;
	}
	struct dirent * my_dirent;
	char * dname;
	char pathname[512];
	int ret_lstat;
	struct stat my_stat;
	while ((my_dirent = readdir(my_DIR)) != NULL) {
		dname = my_dirent->d_name;
		if (strcmp(dname, ".") != 0 && strcmp(dname, "..") != 0) {
			sprintf(pathname, "%s/%s", dir, dname);
			ret_lstat = lstat(pathname, &my_stat);
			if (S_ISREG(my_stat.st_mode))
				remove(pathname);
			else if (S_ISDIR(my_stat.st_mode))
				dremove(pathname); // remove recursively
		}
	}
	rmdir(dir);
	closedir(my_DIR);
	return 0;
}
	

int main (int argc, char * argv[]) {

	// check backpath exists and is a directory

	// find home directory
	struct passwd* pwd = getpwuid(getuid());
	char * homepath = pwd->pw_dir;

	// backup path
	char backpath[256];
	sprintf(backpath, "%s/.backup", homepath);

	struct stat my_stat;
	int ret_st = lstat(backpath, &my_stat);
	if (ret_st != 0) {
		fprintf(stderr, "%s does not exist\n", backpath);
		exit(1);
	} else {
		if (!S_ISDIR(my_stat.st_mode)) {
			fprintf(stderr, "%s is not a directory\n", backpath);
			exit(1);
		}
	}


	// Parse option characters
	int opt;
	char * rDir;
	int pflag = 0, dflag = 0;

	while ((opt = getopt(argc, argv, "pd:")) != -1) {
		switch(opt) {
			case 'p':
				pflag = 1;
				break;
			case 'd':
				dflag = 1;
				rDir = optarg;
				break;
			default:
				fprintf(stderr, "Usage: getbackup [-p] [-d rDir] pathname(s)\n");
				exit(1);
				break;
		}
	}

	char oldpath[512];
	char newpath[512];
	int dir_ind = optind;
	while (dir_ind < argc) {
		sprintf(oldpath, "%s/%s", backpath, argv[dir_ind]);
		//printf("old path: %s\n", oldpath);

		ret_st = lstat(oldpath, &my_stat);
		if (ret_st != 0) {
			fprintf(stderr, "%s does not exist\n", oldpath);
			exit(1);
		}

		if (dflag)
			sprintf(newpath, "%s/%s", rDir, argv[dir_ind]);
			
		else
			sprintf(newpath, "./%s", argv[dir_ind]);

		//printf("new path: %s\n", newpath);

		// copy file/directory
		int ret_copy;
		int ret_rm = 0;
		if (S_ISREG(my_stat.st_mode)) {
			ret_copy = fcopy(oldpath, newpath);
			if (ret_copy != 0) {
				fprintf(stderr, "copy from %s to %s failed\n", oldpath, newpath);
				exit(1); // if copy failed, do not do delete
			}
			if (pflag) {
				ret_rm = remove(oldpath);
			}
		}
		else if (S_ISDIR(my_stat.st_mode)) {
			ret_copy = dcopy(oldpath, newpath);
			if (ret_copy != 0) {
				fprintf(stderr, "copy from %s to %s failed\n", oldpath, newpath);
				exit(1); // if copy failed, do not do delete
			}
			if (pflag) {
				ret_rm = dremove(oldpath);
			}
		}
		if (ret_rm != 0) {
			fprintf(stderr, "%s remove failed\n", oldpath);	
			exit(1);
		}
		dir_ind++;
	}
	return 0;
}

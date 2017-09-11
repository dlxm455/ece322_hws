#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_ARR_CAPACITY 10

struct file_info_struct {
	char * my_d_name;
	char * my_pathname;
	
	mode_t my_st_mode; // file_mode;
	nlink_t my_st_nlink; // num_links;
	uid_t my_st_uid; // owner;
	gid_t my_st_gid; // group;
	off_t my_st_size; // num_bytes;
	//struct timespec my_st_mtimespec;
	time_t my_st_mtime;	
};

struct vector_of_file_info {
	struct file_info_struct * zero_pos_ptr;
	int capacity;
	int size;
};

void vector_push_back(struct vector_of_file_info * v, struct file_info_struct fis) {
	if (v->size >= v->capacity) {
		v->capacity = v->capacity + v->capacity;
		struct file_info_struct * new_arr = (struct file_info_struct *)malloc(sizeof(* new_arr) * v->capacity);
		for (int i = 0; i < v->size; i++) {
			new_arr[i] = v->zero_pos_ptr[i];
		}
		free(v->zero_pos_ptr);
		v->zero_pos_ptr = new_arr; 
	}
	v->zero_pos_ptr[v->size] = fis;
	v->size += 1;
}
	
/*
void createArray (char * pathname, struct file_info_struct * struct_arr) {

	DIR * my_DIR;
	struct dirent * my_dirent;
	struct stat * my_stat;
	
	my_DIR = opendir(pathname);
	while ((my_dirent = readdir(my_DIR)) != NULL) {
		printf("%s\n", my_dirent->d_name);
		//stat(my_dirent->d_name, &myStat);
	}
	closedir(my_DIR);
}
*/	

int main (int argc, char* argv[]) {
	/* 
	 * Parse option characters
	 */
 
	int ecode = 0; /* to hold an error code, 0:success, -1:error */
	
	int opt, Uflag, sflag, lflag, aflag; /* the found option and 4 flags */

	Uflag = 0; /* 0: not present, 1: present */
	sflag = 0;
	lflag = 0;
	aflag = 0; 

	while ((opt = getopt(argc, argv, "Usla")) != -1) {
		switch (opt) {
			case 'U':
				Uflag = 1;
				break;
			case 's':
				sflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			case 'a':
				aflag = 1;
				break;
			default:
				fprintf(stderr, "Usage: %s [-Usla] <path_name>\n", argv[0]);
				ecode = -1;
				return ecode;
		}
	}

	if (Uflag && sflag) {
		fprintf(stderr, "-U and -s are conflict options.\n");
		ecode = -1;
		return ecode;
	}

	/*
	 * Directory entries 
	 */
		
	DIR * my_dir;
	struct dirent * my_dirent;
	struct stat my_stat;
		
	my_dir = opendir(argv[optind]);

	if (my_dir == NULL) {
		fprintf(stderr, "Cannot open the provided directory\n");
		ecode = -1;
		return ecode;
	}
	
	
	if (!sflag && !lflag) ｛
			while ((my_dirent = readdir(my_dir)) != NULL) {
				if (!aflag && my_dirent->d_name[0] == '.') continue;
				printf("%s\n", my_dirent->d_name);
			}
			closedir(my_dir);
			return ecode;
	} 
		
	char path_buf[512]; /* Assume the absolute path will not exceed 512 bytes */	
	struct file_info_struct my_struct;
	struct vector_of_file_info my_vector;
	my_vector.zero_pos_ptr = (struct file_info_struct *) malloc(sizeof(struct file_info_struct) * INIT_ARR_CAPACITY); 
	my_vector.capacity = INIT_ARR_CAPACITY;
	my_vector.size = 0;
	while ((my_dirent = readdir(my_dir)) != NULL) {
		sprintf(path_buf, "%s/%s", argv[optind], my_dirent->d_name);	
		lstat(path_buf, &my_stat);
		
		my_struct.my_d_name = my_dirent->d_name;
		my_struct.my_pathname = path_buf;
		my_struct.my_st_mode = my_stat.st_mode;
		my_struct.my_st_nlink = my_stat.st_nlink;
		my_struct.my_st_uid = my_stat.st_uid;
		my_struct.my_st_gid = my_stat.st_gid;
		my_struct.my_st_size = my_stat.st_size;
		my_struct.my_st_mtime = my_stat.st_mtimespec.tv_sec;
		
		vector_push_back(&my_vector, my_struct);
	}
	closedir(my_dir);
	
	if (!sflag) {
		while (int i = 0; i < my_vector.size(); i++) {
			if (!aflag && my_vector.zero_pos_ptr[i]->my_d_name[0] == '.') continue;
			printf(); //TODO	
		}
	return ecode;
	}
	else {
		vector_sort(&my_vector);
		while (int i = 0; i < my_vector.size(); i++) {
			if (!aflag && my_vector.zero_pos_ptr[i]->my_d_name[0] == '.') continue;
			if (!lflag) printf("%s\n", my_vector.zero_pos_ptr[i]->my_d_name);
			else printf(); //TODO
	}
	/*
	 * Get the name and status for each file in the given directory 
	 */
	
}

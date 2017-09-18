#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define INIT_ARR_CAPACITY 10

/* 
 * Struct to hold information from struct dirent and struct stat for long listing.
 */

struct file_info_struct {
	char * my_d_name; // file or directory name
	//char * my_pathname;
	
	mode_t my_st_mode; // file mode including permission
	nlink_t my_st_nlink; // number of hard links
	uid_t my_st_uid; // owner id
	gid_t my_st_gid; // group id
	off_t my_st_size; // size in bytes
	time_t my_st_mtime;	// modification time
};




/*
 * Struct used to implement an expandable vector to hold the array of file_info_struct.
 */

struct vector_of_file_info {
	struct file_info_struct * zero_pos_ptr; // pointer to the head of the array
	int capacity; // how big the current allocated continuous space is
	int size; // number of elements in the array
};




/*
 * Add a file_info_struct to the vector.
 */

void vector_push_back(struct vector_of_file_info * v, struct file_info_struct fis) {
	if (v->size >= v->capacity) { // if size exceeds capacity
		v->capacity = v->capacity + v->capacity; // double the size of capacity
        // allocate memory for new array with the new capacity
		struct file_info_struct * new_arr = (struct file_info_struct *)malloc(sizeof(* new_arr) * v->capacity);
		for (int i = 0; i < v->size; i++) {
			new_arr[i] = v->zero_pos_ptr[i]; // copy data from old array to new array
		}
		free(v->zero_pos_ptr); // free memory of old array
		v->zero_pos_ptr = new_arr; // head pointer points to the new array
	}
	v->zero_pos_ptr[v->size] = fis; // add the given struct to array
	v->size += 1; // increase size by 1
}




/*
 * Implement quicksort based on file (or directory) name of the file_info_struct array.
 */

void vector_sort(struct vector_of_file_info * v, int first, int last) {
	if (first < last) {
		struct file_info_struct pivot, tmp;
		int i, k;

		/* partition */
		i = first; pivot = v->zero_pos_ptr[first];
		for (k = first + 1; k <= last; ++k) {
			if (strcmp(v->zero_pos_ptr[k].my_d_name, pivot.my_d_name) <= 0) {
				++i; 
				tmp = v->zero_pos_ptr[k];
				v->zero_pos_ptr[k] = v->zero_pos_ptr[i];
				v->zero_pos_ptr[i] = tmp;
			}
		}
		v->zero_pos_ptr[first] = v->zero_pos_ptr[i];
		v->zero_pos_ptr[i] = pivot;

		/* sort by recursion */
		vector_sort(v, first, i - 1);
		vector_sort(v, i + 1, last);
	}
}
void print_long_format(struct file_info_struct * fis) {
	mode_t m = fis->my_st_mode;
	printf((S_ISDIR(m)) ? "d" : "-");
	printf((m & S_IRUSR) ? "r" : "-");
	printf((m & S_IWUSR) ? "w" : "-");
	printf((m & S_IXUSR) ? "x" : "-");
	printf((m & S_IRGRP) ? "r" : "-");
	printf((m & S_IWGRP) ? "w" : "-");
	printf((m & S_IXGRP) ? "x" : "-");
	printf((m & S_IROTH) ? "r" : "-");
	printf((m & S_IWOTH) ? "w" : "-");
	printf((m & S_IXOTH) ? "x" : "-");
	printf("  ");
	
	printf("%d ", fis->my_st_nlink);
	
	struct passwd * pwd;
	if ((pwd = getpwuid(fis->my_st_uid)) != NULL) 
		printf("%8.8s", pwd->pw_name);
	else 
		printf("%8d", fis->my_st_uid);
	
	struct group *grp;
	if ((grp = getgrgid(fis->my_st_gid)) != NULL)
		printf("%8.8s", grp->gr_name);
	else
		printf("%8d", fis->my_st_gid);

	printf("%8lld ", fis->my_st_size); // TODO: check format	
	
	printf("%s ", ctime(&fis->my_st_mtime));
	
	printf("%s", fis->my_d_name);
	
	printf("\n");

}

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
	
	
	if (Uflag && !lflag) {
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
		//my_struct.my_pathname = path_buf;
		my_struct.my_st_mode = my_stat.st_mode;
		my_struct.my_st_nlink = my_stat.st_nlink;
		my_struct.my_st_uid = my_stat.st_uid;
		my_struct.my_st_gid = my_stat.st_gid;
		my_struct.my_st_size = my_stat.st_size;
		my_struct.my_st_mtime = my_stat.st_mtimespec.tv_sec;
		
		vector_push_back(&my_vector, my_struct);
	}
	closedir(my_dir);
	
	if (sflag || (!sflag && !Uflag)) vector_sort(&my_vector, 0, my_vector.size - 1);
	for (int i = 0; i < my_vector.size; i++) {
		if (!aflag && my_vector.zero_pos_ptr[i].my_d_name[0] == '.') continue;
		if (!lflag) printf("%s\n", my_vector.zero_pos_ptr[i].my_d_name);
		else print_long_format(&my_vector.zero_pos_ptr[i]);
	}

}

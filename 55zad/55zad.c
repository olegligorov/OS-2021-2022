#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed))triplet_t;

int main(int argc, char* argv[]){
	if (argc != 4)
		errx(1, "Invalid arguments");

	const char* patch = argv[1], *f1 = argv[2];
	char* f2 = argv[3]; 
	
	struct stat st_patch;
	if ((stat(patch,&st_patch)) == -1)
		err(1, "Unable to stat %s", patch);

	triplet_t triplet;
//	if (st_patch.st_size % sizeof(triplet) != 0)
//		errx(1, "File %s is corrupted", patch);

	int fd_p=open(patch, O_RDONLY);
	if (fd_p == -1)
		err(1, "Unable to open %s", patch);

	int fd1=open(f1, O_RDONLY);
	if (fd1 == -1)
		err(1, "Unable to open %s", f1);

	int fd2=open(f2, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd2 == -1)
		err(1, "Unable to open %s", f2);

	int read_sz;
	uint8_t buffer[2048];
	int write_sz;
	while ((read_sz=read(fd1, &buffer, sizeof(buffer)))> 0) {
		write_sz=write(fd2, &buffer, read_sz);
		if (write_sz != read_sz){
			errx(1, "Unable to write to file %s", f2);
		}
	}
	if (read_sz == -1)
		err(1, "Unable to read from file %s", f1);
	//we copied f1 in f2
	
	uint8_t c;
	int read_c;
	while ((read_sz=read(fd_p,&triplet,sizeof(triplet))) == sizeof(triplet)) {

		if (lseek(fd2, triplet.offset, SEEK_CUR) == -1)
			err(1, "Error while lseeking file %s", f2);
		if ((read_c=read(fd2,&c,sizeof(c))) == -1){
			err(1, "Error while reading file %s", f2);
		}

		if (c == triplet.original) {
			if ((write_sz=write(fd2,&triplet.new,sizeof(triplet.new))) != sizeof(triplet.new)) {
				err(1, "Error while writing in file %s", f2);
			}
		}
		// else	errx(1, "The original byte does not match");
	}
	if (read_sz == -1)
		err(1, "Error while reading from file %s", patch);

	close(fd1);
	close(fd2);
	close(fd_p);
	exit(0);
}


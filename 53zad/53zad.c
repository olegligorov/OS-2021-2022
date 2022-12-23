#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <sys/stat.h>

typedef struct {
	uint16_t offset;
	uint8_t original;
	uint8_t new;
}__attribute__((packed)) triple_t;

int main(int argc, char* argv[]) {
	
	if (argc != 4)
		errx(1, "Invalid arguments");

	char* f1=argv[1];
	char* f2=argv[2];
	char* patch=argv[3];

	struct stat st1, st2;
	if (stat(f1,&st1) == -1)
		err(1, "Unable to stat file %s", f1);
	if (stat(f2,&st2) == -1)
		err(1, "Unable to stat file %s", f2);

	if (st1.st_size != st2.st_size || st1.st_size > UINT16_MAX)
		errx(1, "Input files are corrupted");

	int fd1=open(f1, O_RDONLY);
	if (fd1 == -1)
		err(1, "Unable to open file %s", f1);
	int fd2=open(f2, O_RDONLY);
	if (fd2 == -1)
		err(1, "Unable to open file %s", f2);
	int fd3=open(patch, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd3 == -1)
		err (1, "Unable to open file %s", patch);

	uint8_t a,b;
	uint16_t offset=1;
	int r1,r2;
	triple_t triple;
	while (((r1=read(fd1,&a, sizeof(a))) == sizeof(a)) && ((r2=read(fd2, &b, sizeof(b))) == sizeof(b))) {
		if (a != b) {
			triple.offset=offset;
			triple.original=a;
			triple.new=b;

			int wr;
			if ((wr=write(fd3,&triple,sizeof(triple))) != sizeof(triple)) {
				err(1, "Unable to write to file %s", patch);
			}
		}
		++offset;
	}
	
	if (r1 == -1 || r2 == -1)
		err(1, "Unable to read from file");
	
	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}


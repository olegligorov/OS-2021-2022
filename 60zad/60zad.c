#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h> 

int cmp(const void* a, const void *b) {
	return (*(uint16_t*)a - *(uint16_t*)b);
}

int main(int argc, char* argv[]) {
	if (argc != 3)
		errx(1, "Invalid arguments");

	struct stat st;
	if (stat(argv[1], &st) == -1)
		err(1, "Can not stat file %s", argv[1]);
	uint32_t size = st.st_size % sizeof(uint16_t);
	if (size > 65535)
		errx(1, "Size %s is too big", argv[1]);

	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
		err(1, "Unable to open file %s",argv[1]);
	int fd2=open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd2 == -1)
		err(1, "Unable to open file %s", argv[2]);

	uint32_t half = size / 2;
	uint32_t rhalf = size - half;
	uint16_t* arr = (uint16_t*)malloc(half * sizeof(uint16_t));
	
	long unsigned int read_sz;
	long unsigned int write_sz;

	int ft1=open("temp1.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (ft1 == -1)
		err(1, "Unable to open temp1.txt");
	int ft2=open("temp2.txt", O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (ft2 == -1)
		err(1, "Unable to open temp2.txt");

	read_sz=read(fd1, arr, half * sizeof(uint16_t));
	if (read_sz != half * sizeof(uint16_t)) {
		err(1, "Error while reading from file");
	}

	qsort(arr, half, sizeof(uint16_t), cmp);
	
	write_sz=write(ft1, &arr, half * sizeof(uint16_t));
	if (write_sz != half * sizeof(uint16_t))
		err(1, "Unable to write to temp1");

	uint16_t* rarr=(uint16_t*)malloc(half * sizeof(uint16_t));
	read_sz=read(fd1, rarr, rhalf * sizeof(uint16_t));
	if (read_sz != rhalf * sizeof(uint16_t))
		err(1, "Error while reading from %s", argv[1]);
	
	qsort(rarr,rhalf, sizeof(uint16_t), cmp);

	write_sz=write(ft2, rarr, rhalf * sizeof(uint16_t));
	if (write_sz != rhalf * sizeof(uint16_t))
		err(1, "Unable to write to temp2");
	free(rarr);

	if (lseek(ft1, 0, SEEK_SET) == -1)
		err(1, "Unable to lseek temp1");
	if (lseek(ft2, 0, SEEK_SET) == -1)
		err(1, "Unable to lseek temp2");

	uint16_t a, b;
	int read_sz2=-1;

	while((read_sz=read(ft1, &a, sizeof(a))) > 0 &&
		   	(read_sz2=read(ft2, &b, sizeof(b))) > 0) {
		if (a <= b) {
			if (write(fd2, &a, sizeof(a)) == -1)
				err(1, "Unable to write to file %s", argv[2]);
			if (lseek(ft2, -1 * sizeof(b), SEEK_CUR))
				err(1, "Unable to lseek temp2");
		} else {
			if (write(fd2, &b, sizeof(b)) == -1)
				err(1, "Unable to write to file %s", argv[2]);
			if (lseek(ft1, -1 * sizeof(a), SEEK_CUR))
				err(1, "Unable to lseek temp1");
		}
	}

	if (read_sz == sizeof(a)) {
		if (write(fd2, &a, sizeof(a)) == -1)
			err(1, "Unable to write to fiel %s",argv[2]);
	}

	if (read_sz == -1 || read_sz2 == -1)
		err(1, "Unable to read from file");

	while ((read_sz=read(ft1,&a,sizeof(a))) > 0) {
		if (write(fd2, &a, sizeof(a)) == -1)
			err(1, "unable to write to file %s", argv[2]);
	}
	if (read_sz == -1)
		err(1, "Error while reading from file");

	while ((read_sz2=read(ft2, &b, sizeof(b))) > 0) {
		if (write(fd2, &b, sizeof(b)) == -1)
			err(1, "Unable to write to file %s", argv[2]);
	}
	if (read_sz2 == -1)
		err(1, "Unable to read from file");

	unlink("temp1.txt");
	unlink("temp2.txt");
	close (fd1);
	close (fd2);
	close (ft1);
	close (ft2);
	
	exit(0);
}

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

int cmp(const void *a, const void *b) {
	return (*((uint32_t*)a) - *((uint32_t*)b));
}

int main(int argc, char* argv[]) {
	if (argc != 3)
		errx(1, "invalid number of arguments");

	char *bin = argv[1], *sorted=argv[2];

	struct stat st;
	if (stat(bin, &st) == -1)
		err(1, "Error while stat %s", bin);
	if (st.st_size % sizeof(uint32_t) != 0)
		errx(1, "File %s is corrupted", bin);

	uint32_t length = st.st_size / sizeof(uint32_t);
	uint32_t lhalf = length/2;
	uint32_t rhalf = length - lhalf;

	uint32_t* arr = (uint32_t*) malloc(lhalf * sizeof(uint32_t));
	if (!arr)
		err(1, "Not enough memory");

	int fd1 = open(bin , O_RDONLY);
	if (fd1 == -1)
		err(1, "Unable to open file %s",bin);

	int t1 = open("temp1", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (t1 == -1)
		err(1, "unable to open file temp1");
	
	uint32_t res = read(fd1, arr, lhalf * sizeof(uint32_t));
	if (res !=  lhalf * sizeof(uint32_t))
		err(1, "Unable to read file %s", bin);
	
	qsort(arr, lhalf, sizeof(uint32_t), cmp);

	res=write(t1, arr, lhalf*sizeof(uint32_t));
	if (res != (uint32_t) lhalf * sizeof(uint32_t))
		err(1, "unable to write to temp1");

	free(arr);
	uint32_t* rarr=(uint32_t*)malloc(rhalf * sizeof(uint32_t));
	
	res=read(fd1, rarr, rhalf*sizeof(uint32_t));
	if (res != rhalf*sizeof(uint32_t))
		err(1, "Unable to read second half of the file %s", bin);

	qsort(rarr, rhalf, sizeof(uint32_t), cmp);
	
	int t2=open("temp2", O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (t2 == -1)
		err(1, "unable to open file temp2");

	res=write(t2, rarr, rhalf*sizeof(uint32_t));
	if (res != rhalf*sizeof(uint32_t))
		err(1, "Unable to write to temp2");

	free(rarr);
	close(fd1);

	if (lseek(t1, 0, SEEK_SET) == -1)
		err (1, "Unable to lseek temp1");
	if (lseek(t2, 0, SEEK_SET) == -1)
		err (1, "Unable to lseek temp2");

	int fd2=open(sorted, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd2 == -1)
		err(1, "Unable to open file %s", sorted);
	
	uint32_t a,b;
	int rd1, rd2;
	while(((rd1= read(t1, &a, sizeof(a))) == sizeof(a)) && ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b))) {
		if ( a <= b) {
			if (write(fd2,&a,sizeof(a)) != sizeof(a))
				err(1, "Unable to write to file %s",sorted);
			if (lseek(t2, -1*sizeof(b), SEEK_CUR) == -1)
				err(1, "unable to lseek file temp2");
		}
		else {
			if (write(fd2, &b, sizeof(b)) != sizeof(b))
				err(1, "unable to write to file %s", sorted);
			if (lseek(t1, -1*sizeof(a), SEEK_CUR) == -1)
				err(1, "unable to lseek file temp1");
		}
	}
	if (rd1 == sizeof(a)){
		write(fd2,&a,sizeof(a));
	}
	if (rd1 == -1 || rd2 == -1){
		err(1, "Error while reading from temp file");
	}

	//now for the rest
	
	while ((rd1 = read(t1, &a, sizeof(a))) == sizeof(a)) {
		if (write(fd2, &a, sizeof(a)) != sizeof(a)) 
			err(1, "unable to write to sorted file");
	}
	if (rd1 == -1)
		err(1, "Unable to read from temp1");

	while ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b)) {
		if (write(fd2, &b, sizeof(b)) != sizeof(b))
			err(1, "unable to write to sorted file");
	}
	if (rd2 == -1)
		err(1, "unable to read from temp2");

	unlink("temp1");
	unlink("temp2");
	close(t1);
	close(t2);
	close(fd2);
	exit(0);
}


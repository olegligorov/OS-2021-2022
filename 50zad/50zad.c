#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <err.h>

typedef struct {
	uint32_t start;
	uint32_t length;
} pair_t;

int main() {

	int fd1,fd2,fd3;
	char *f1 = "f1", *f2="f2", *f3="f3";

	if ((fd1 = open(f1, O_RDONLY)) == -1) {
		err(1, "Error cannot open file %s", f1);
	}
	if ((fd2 = open(f2, O_RDONLY)) == -1) {
		err(1, "Error cannot open file %s", f2);
	}

	if ((fd3 = open(f3, O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1) {
		err(1, "Error cannot open file %s", f3);
	}

	int rd1 = -1;
	pair_t pair;
	while((rd1 = read(fd1,&pair,sizeof(pair))) > 0) {
		uint32_t from = pair.start;
		uint32_t length = pair.length;
	
		if (lseek(fd2, from * sizeof(uint32_t), SEEK_SET) == -1) {
			err(1, "Error while lseeking file %s", f2);
		}

		uint32_t num;
		uint32_t count = 1;
		int rd2 = -1;
		while((rd2 = read(fd2, &num, sizeof(num))) && count<=length) {
			count++;
			int wr = write(fd3, &num, rd2);
			if (wr != rd2) {
				err(1, "Error while writing to file %s", f3);
			}
		}
		if (rd2 == -1)
			err(1, "Error while reading from file %s", f2);
	}
	if (rd1 == -1)
		err(1, "Error while reading from file %s", f1);

	if (close(fd1) == -1)
		err(1, "Unable to close file %s", f1);
	if (close(fd2) == -1)
		err(1, "Unable to close file %s", f2);
	if (close(fd3) == -1)
		err(1, "Unable to close file %s", f3);

}


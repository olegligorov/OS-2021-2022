#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

uint16_t mypow(int num, int degree) {
	uint16_t res = 1;
	for (int i = 0; i < degree; ++i) {
		res *= num;
	}
	return res;
}

int main(int argc, char* argv[]) {
	if (argc != 3)
	  errx(1, "Invalid arguments");
	
	int fd1=open(argv[1], O_RDONLY);
	if (fd1 == -1)
		err(1, "Error while opening file %s",argv[1]);
	int fd2=open(argv[2], O_CREAT | O_TRUNC | O_WRONLY,0666);
	if (fd2 == -1)
		err(1, "Error while opening file %s",argv[2]);

	uint8_t input;
	uint16_t encode = 0;
	int rs;
	while ((rs=read(fd1, &input, sizeof(input))) == sizeof(input)) {
		for (int i = 0; i < 8; ++i) {
			if ((input & (1 << i)) != 0) {
				uint16_t temp = 1 * mypow(2, i + 1) + 0 * mypow(2, i);
				encode += (temp << i);
			} else {
				uint16_t temp = 0 * mypow(2, i + 1) + 1 * mypow(2, i);
				encode += (temp << i);
			}
		}
		if (write(fd2, &encode, sizeof(uint16_t)) != sizeof(uint16_t))
			err(1, "Error while writing");
	}
	if (rs == -1)
		err(1, "Error while reading from file");
	close(fd1);
	close(fd2);
	exit(0);
}

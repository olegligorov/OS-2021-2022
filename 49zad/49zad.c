#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc != 2)
		errx(1, "Invalid number of arguments. Usage :%s <file.bin>", argv[0]);

	char* file = argv[1];

	uint64_t counting[256] = { 0 };
	uint16_t level[256];

	for (uint16_t j = 0; j < 256; ++j) {
		level[j] = 1;
	}

	int fd = open(file, O_RDWR);
	if (fd == -1)
		err(1, "Error while opening file %s", file);

	ssize_t read_sz;
	uint8_t b;

	while((read_sz = read(fd, &b, sizeof(b))) == sizeof(b)) {
		counting[b]++;
		if (counting[b] == UINT64_MAX) {
			counting[b] = 0;
			level[b]++;
		}
	}
	if (read_sz == -1) {
		close(fd);
		err(1, "Error while reading file %s", file);
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		close(fd);
		err(1, "Error while lseek file %s", file);
	}

	for (int i = 0; i < 256; ++i) {
		while (level[i]){
			b = i;
			while(counting[i]) {
				if (write(fd, &b, sizeof(b)) != sizeof(b)) {
					close(fd);
					err(1, "Error while writing into file %s", file);
				}
				counting[i]--;
			}
			level[i]--;
			counting[i]=UINT64_MAX;
		}
	}

	if (close(fd) == -1)
		err(1, "Unable to close file");
	exit(0);
}

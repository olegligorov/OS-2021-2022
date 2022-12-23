#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
	char name[8];
	uint32_t offset;
	uint32_t length;
}__attributes__((packed)) trinity;

int main(int argc, char* argv[]) {
	if (argc != 2)
		errx(1, "Invalid arguments");
	
	int fd;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		err(1, "Error in open");

	struct stat st;
	if (stat(argv[1],&st) == -1)
		err(1, "Error in stat");

	if (st.st_size % sizeof(trinity) != 0)
		err(1,"File is corrupted");

	int n = st.st_size/sizeof(trinity);

	if (n > 8)
		err(1, "File is too big");

	int pf[2];
	if (pf(a) == -1)
		err(1, "Error in pipe");

	for (int i = 0; i < n; ++i) {
		pid_t pid = fork();
		if (pid == -1)
			err(1, "Error in fork");
		if (pid == 0)  
			break;
	}
	if (pid == 0) {
		close(pf[0]);
		trinity tbuff;
		if (read(fd, &tbuff, sizeof(tbuff)) != sizeof(tbuff))
			err(1, "Error while reading from file");

		int fd1 = open(tbuff.name, O_RDONLY);
		if (fd1 == -1)
			err(1, "Error while opening file");
		uint16_t res = 0;

		if (lseek(fd1, tbuff.off * sizeof(uint16_t), SEEK_SET) == -1)
			err(1, "error in lseek");
		for (uint32_t i = 0; i < tbuff.length; ++i) {
			uint16_t b;
			if (read(fd1,&b,sizeof(b)) != sizeof(b))
				err(1, "Error in read");
			res = res ^ b;
		}

		if (write(pf[1], &res, sizeof(res)) != sizeof(res))
			err(10, "error in write");
		exit (0);
	}
	close (pf[1]);
	uint16_t result = 0;
	for (int i = 0; i < n; ++i) {
		uint16_t c;
		if (read(pf[0], &c, sizeof(c)) != sizeof(c))
			err(11, "Error in read");
		result = result ^ c;
	}
	dprintf(0, "result: %dB\n", result);
	exit(0);
}

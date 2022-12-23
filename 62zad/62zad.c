#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdint.h>
#include <err.h>

typedef struct {
	uint32_t magic;
	uint8_t version;
	uint8_t data_version;
	uint16_t count;
	uint32_t r1;
	uint32_t r2;
} header;

typedef struct {
	uint16_t offset;
	uint8_t original;
	uint8_t new;
} first_version; //0x00

typedef struct {
	uint32_t offset;
	uint16_t original;
	uint16_t new; 
} second_version; //0x01

int main(int argc, char* argv[]) {
	if (argc != 4)
		errx(1, "Invalid arguments");

	header h;
	int fdp = open(argv[1], O_RDONLY);
	if (fdp == -1)
		err(1, "Error while opening %s", argv[1]);

	int fd1 = open(argv[2], O_RDONLY);
	if (fd1 == -1)
		err(1, "Error while opening %s", argv[2]);
	int fd2 = open(argv[3], O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd2 == -1)
		err(1, "Error while opening %s", argv[3]);

	int rs = read(fdp, &h, sizeof(h));
	if (rs != sizeof(h))
		err(1, "Error while reading from %s", argv[1]);

	if (h.magic != 0xEFBEADDE || h.version != 0x01)
		errx(1, "Invalid version or magic key!");

	if (h.data_version == 0x00) {
		uint8_t buff;
		while ((rs=read(fd1, &buff, sizeof(buff))) > 0) {
			if (write(fd2, &buff, sizeof(buff)) != sizeof(buff))
				err(1, "Error while writing");
		}
		if (rs == -1)
		 err(1, "Error while reading");	

		first_version v1;
		while ((rs = read(fdp, &v1, sizeof(v1))) == sizeof(v1)) {
			if (lseek(fd1, v1.offset, SEEK_SET) == -1)
				err(1, "Invalid offset");
			int rs2 = read(fd1, &buff, sizeof(buff));
			if (rs2 == -1)
				err(1,"Error while reading");
			if (buff == v1.original) {
				if (lseek(fd2, v1.offset, SEEK_SET) == -1)
					err(1, "Invalid offset");
				if (write (fd2, &v1.new, sizeof(v1.new)) != sizeof(v1.new))
					err(1, "Error while writing!");
			}
		}
		if (rs == -1)
			err(1, "Error while reading");
	} else if (h.data_version == 0x01) {
		uint16_t buff;
		while ((rs=read(fd1, &buff, sizeof(buff))) > 0) {
			if (write(fd2, &buff, sizeof(buff)) != sizeof(buff))
				err(1, "Error while writing");
		}

		if (rs == -1)
			err(1, "Error while reading");
		second_version v2;
		while ((rs = read(fdp, &v2, sizeof(v2))) == sizeof(v2)) {
			if (lseek(fd1, v2.offset, SEEK_SET) == -1)
				err(1, "Invalid offset");	
			int rs2= read(fd1, &buff, sizeof(buff));
			if (rs2 == -1)
				err(1, "Error while reading");
			if (buff == v2.original) {
				if (lseek(fd2, v2.offset, SEEK_SET) == -1)
					err(1, "invalid offset");
				if (write(fd2, &v2.new, sizeof(v2.new)) != sizeof(v2.new))
					err(1, "Error while writing");
			}
		}
		if (rs == -1)
			err(1, "Error while reading");
	}
	close(fdp);
	close(fd1);
	close(fd2);
	exit(0);
}

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	if (argc != 3)
		errx(1, "Invalid arguments");

	int fd1=open(argv[1], O_RDONLY);
	if (fd1 == -1)
		err(1, "Can not open file %s", argv[1]);
	int fd2=open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd2 == -1)
		err(1, "Can not open file %s", argv[2]);

	struct stat st;
	if (stat(argv[1],&st) == -1)
		err(1, "Unable to stat file %s",argv[1]);

	uint32_t size = st.st_size / sizeof(uint16_t);
	if (size > 524288)
		errx(1, "File is too big");

	int rs;
	uint16_t buff;
	dprintf(fd2, "uint32_t arrN=%d;\n", size);
	dprintf(fd2, "uint16_t arr[] = {");

	while((rs=read(fd1, &buff, sizeof(buff))) == sizeof(buff)) {
		dprintf(fd2, "%d,",buff);
	}
	if (rs == -1)
		err(1, "Unable to read from file");
	dprintf(fd2, "};");
	
	close(fd1);
	close(fd2);
	exit (0);
}

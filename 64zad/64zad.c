#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdint.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	if (argc != 3)
		errx(1, "invalid arguments");

	int scl=open(argv[1], O_RDONLY);
	if (scl == -1)
		err(1, "Error while opening file %s", argv[1]);
	int sdl=open(argv[2], O_RDONLY);
	if (sdl == -1)
		err(1, "Error while opening file %s", argv[2]);


	int fd=open("high.sdl", O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		err(1, "Error while opening file high.sdl");
	uint16_t buff;
	uint8_t level;
	int rs;
	int ws;
	while ((rs = read(scl, &level, sizeof(level))) > 0) {
		rs=read(sdl, &buff, sizeof(buff));
		if (rs != sizeof(buff))
			err(1, "Error while reading");
		if (level == 1) {
			if ((ws=write(fd,&buff,sizeof(buff))) != sizeof(buff))
				err(1, "Error while writing in high.sdl");
		}
	}
	if (rs == -1) {
		err (1, "Error while reading");
	}	
	close (fd);
	close (sdl);
	close (scl);
	exit(0);
}

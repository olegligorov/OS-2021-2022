#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void my_read(int fd,const char* file) {
	int read_sz;
	int write_sz;
	
	char c;
	while((read_sz=read(fd, &c, sizeof(c))) > 0) {
		if ((write_sz=write(1, &c, sizeof(c))) == -1)
			err(1, "Unable to write to stdout");	
	}
	if (read_sz == -1)
		err(1, "Unable to read from %s", file);
	close(fd);
}

int main(int argc, char* argv[])
{
	if (argc == 1){
		my_read(0, "STDIN");
		exit(0);
	}

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i],"-") == 0) {
			my_read(0, "STDIN");
			continue;
		}

		const char* file = argv[i];
		int fd = open(file, O_RDONLY);
		if (fd == -1)
			err(1, "unable to open file %s", file);

		my_read(fd, file);
	}

	exit(0);
}

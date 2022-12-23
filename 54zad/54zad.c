#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>

int counter=1;
int option=0;

void my_read(int fd, const char *from) {
	char c;
	int read_sz;

	int newLine=1;

	while ((read_sz=read(fd, &c, 1)) == 1) {
		if (option){
			if (newLine){
				dprintf(1, "%d %s", counter, &c);
				newLine=0;
				counter++;
			} else
				if (write(1, &c, 1) == -1)
					err(1, "Unable to write to stdout");
			if ( c == '\n') newLine = 1;
		} else
			if (write(1,&c,1) == -1)
				err(1, "unable to write to stdout");
	}
	if (read_sz == -1)
		err(1, "Error while reading from %s", from);
}

int main(int argc, char* argv[]){

	if (argc == 1){
		my_read(0, "STDIN");
		exit(0);
	}
	
	if (argc == 2 && strcmp(argv[1], "-n") == 0) {
		option = 1;
		my_read(0, "STDIN");
		exit(0);
	}
	

	if (strcmp(argv[1], "-n") == 0) {
		option = 1;
	}

	for (int i = 2; i < argc; ++i) {
		if (strcmp("-", argv[i]) == 0) {
			my_read(0, "STDIN");
			continue;
		}
		const char* file = argv[i];

		struct stat st;
		if (stat(file,&st) == -1)
			err(1, "Could not stat file %s", file);

		if (!(st.st_mode & S_IRUSR)) {
			warnx("File %s is not readable", file);
			continue;
		}

		int fd = open(file, O_RDONLY);
		if (fd == -1)
			err(1, "unable to open file %s", file);

		my_read(fd, file);
	}
	exit(0);
}

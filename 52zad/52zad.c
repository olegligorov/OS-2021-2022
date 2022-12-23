#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>

typedef struct {
uint16_t offset;
uint8_t length;
uint8_t reserved; //wont be used
} command_t;

int main(int argc, char* argv[]) {

	if (argc != 5)
		errx(1, "Invalid number of arguments");

	int fd1,fd2,fd3,fd4;
	const char *f1_dat = argv[1], *f1_idx = argv[2];
	char *f2_dat = argv[3], *f2_idx = argv[4];

	command_t command1;

	struct stat f1_dat_stat;
	if (stat(f1_dat, &f1_dat_stat) == -1) {
		err(2, "error while stat %s", f1_dat);
	}
	if (f1_dat_stat.st_size == 0)
		exit(0);

	struct stat f1_idx_stat;
	if (stat(f1_idx, &f1_idx_stat) == -1)
		err(2, "Error while stat %s", f1_idx);
	
	
	if (f1_idx_stat.st_size % sizeof(command1) != 0){
		errx(4, "error with length of file %s", f1_idx);
	}

	if ((fd1=open(f1_dat, O_RDONLY)) == -1)
		err(1, "Unable to open file %s", f1_dat);

	if ((fd2=open(f1_idx, O_RDONLY)) == -1)
		err(1, "Unable to open file %s", f1_idx);

	if ((fd3=open(f2_dat, O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1)
		err(1, "Unable to open file %s", f2_dat);

	if ((fd4=open(f2_idx, O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1)
		err(1, "Unable to open file %s", f2_idx);

	uint8_t buff;
	command_t command2;
	int pos=0;
	int len=0;
	int read_sz = 0;
	int firstLet = 1;

	while ((read_sz=read(fd2, &command1, sizeof(command1))) == sizeof(command1)) {
		
		firstLet = 1;
		len = 0;

		if (lseek(fd1, command1.offset, SEEK_SET) == -1)
			err(1, "Error while lseek %s", f1_dat);

		int read_sz2;
		while (read_sz2=read(fd1, &buff, sizeof(buff)) > 0){
			if ((buff < 'A' || buff > 'Z') && firstLet)
				break;
			command2.offset=pos;
			
			int wr_size = write(fd3, &buff, sizeof(buff));
			if (wr_size == -1)
				err(1, "Error while writing in file %s", f2_dat);
			
			firstLet=0;
			len++;
			
			if (len==command1.length) {
				command2.length=len;
				command2.reserved=0;
				pos += len;
				wr_size = write(fd4, &command2, sizeof(command2));
				if (wr_size == -1)
					err(1, "Error while writing in file %s", f2_idx);
				break;
			}
		}
		if (read_sz2 == -1)
			err(1, "Error while reading from file %s", f1_idx);
	}
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);

}

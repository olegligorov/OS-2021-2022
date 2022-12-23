#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
uint32_t uid;
uint16_t a;
uint16_t b;
uint32_t start;
uint32_t end;
} user_t;

int main(int argc, char* argv[]) {
	if (argc != 2)
		errx(1, "Invalid arguments");

	int fd=open(argv[1], O_RDONLY);
	if (fd == -1)
		err(1, "Unable to open file %s", argv[1]);

	uint32_t counter = 0;
	uint64_t time = 0;

	user_t buff;
	int rs;
	int index = 0;
	uint32_t users[2048] = { 0 };
	uint32_t timearr[2048] = { 0 };

	while ((rs = read(fd, &buff, sizeof(buff))) > 0) {
		time += buff.end - buff.start;
		users[index] = buff.uid;
		timearr[index] = buff.end - buff.start;
		index++;
		counter++;
	}
	if (rs == -1)
		err(1, "Unable to read from file %s", argv[1]);

	time = time / counter; //time is x_

	lseek(fd, 0, SEEK_SET);
	uint64_t d = 0;
	uint32_t n = 0;
	while ((rs = read(fd, &buff, sizeof(buff))) > 0) {
		d = (buff.end - buff.start - time) * (buff.end - buff.start - time);
		++n;
	}
	if (rs == -1)
		err(1, "Unable to read from file %s", argv[1]);
	d = d / n;

	lseek(fd, 0, SEEK_SET);
	while ((rs = read(fd, &buff, sizeof(buff))) > 0) {
		uint32_t max = -1;
		uint64_t time_spent = buff.end - buff.start;
		if (time_spent * time_spent > d) {
			for (int i = 0; i <= index; ++i) {
				if (users[i] == buff.uid) {
					if (timearr[i] > max) {
						max = timearr[i];
					}
				}
			}
		}
		if (write (1, &buff.uid, sizeof(buff.uid)) != sizeof(buff.uid)) {
			err(1, "Unable to write");
		}	
		if (write(1, &max, sizeof(max)) != sizeof(max))
			err(1, "Unable to write");
	}
	if (rs == -1)
		err(1, "Unable to read from file %s", argv[1]);
}

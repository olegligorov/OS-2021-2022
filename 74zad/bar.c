#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

int main(int argc, char* argv[]) {
	if (argc != 2)
		err(1, "Invalid arguments");

	int fd = open("/tmp/74fifo",O_RDONLY);
	if (fd == -1)
		err(1, "Opening failure");

	dup2(fd,0);
	if (execlp(argv[1], argv[1], (char*)NULL) == -1)
		err(1, "execlp failed");
}

#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	if (argc != 2)
		err(1, "Wrong number of arguments");
	
	char* myfifo = "/tmp/74fifo";
	if (mkfifo(str, 0666) == -1)
		err(1, "mkfifo failed");

	int fd = open(myfifo, O_WRONLY);
	if (fd == -1)
		err(1, "Opening failure!");

	dup2(fd, 1);

	if (execlp("cat", "cat", argv[1], (char*)NULL) == -1)
		err(1, "Execlp failed");
}

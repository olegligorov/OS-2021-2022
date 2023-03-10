#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(void) {
	while(1) {
		if (write(1, "Command: ", sizeof("Command: ")) == -1)
			err(1, "Unable to write");

		char buff[1000];
		ssize_t read_sz=read(0, &buff, sizeof(buff));
		if (read_sz == -1)
			err(1, "Could not read from stdin");
	
		buff[read_sz - 1] = '\0';

		if (strcmp(buff, "exit") == 0)
			break;

		const char* bin = "/bin/";
		char cmd[10000];
		strcpy(cmd,bin);
		strcat(cmd,buff);

		pid_t pid = fork();
		if (pid == -1)
			err(1, "Could not fork");
		if (pid == 0) {
			if (execlp(cmd, cmd, (char*)NULL) == -1)
				err(1, "Execlp failed!");
		}
		if (pid > 0) {
			if (wait(NULL) == -1)
				err(1, "Could not wait for process %s to finish", cmd);
		}
	}
	exit (0);
}

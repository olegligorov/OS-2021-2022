#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[]) {
	if (argc < 3)
		err(1, "Wrong number of arguments!");
	int fd = open("run.log", O_CREAT | O_TRUNC | O_WRONLY, 0666);
	int time_limit = argv[1][0] - '0';
	int second_time=0;
	while (1) {
		int pf[2];
		if (pipe(pf) == -1) 
			err(1, "Pipe failed!");

		int finish;
		int exit_status;
		pid_t pid = fork();
		if (pid == -1)
			err(1, "Fork failed");
		if (pid == 0) {
			int start = time(NULL);
			close(pf[0]);
			if (write(pf[1], &start, sizeof(start)) != sizeof(start))
				err(1, "Error while writing");
			if (execvp(argv[2], argv + 2) == -1)
				err(1, "Error while executing command");
		}
		close (pf[1]);
		int status;
		wait(&status);
		finish=time(NULL);
		exit_status=WEXITSTATUS(status);
		
		int start;
		if (read(pf[0], &start, sizeof(start)) != sizeof(start))
			err(1, "Error while reading");

		dprintf(1, "%d %d %d\n", start, finish, exit_status);

		if (exit_status != 0 && finish - start < time_limit) {
			if(second_time)
				exit(0);
			else {
				second_time = 1;
			}
		}else
		   second_time = 0;	
		close(pf[0]);
	}
}

#include <stdlib.h>
#include <unistd.h>
#include <err.h>

void exec_cmd(char args[][5], char* cmd) {
	pid_t pid = fork();
	if (pid == 0) {
		char *execv_args[4];
		execv_args[0] = cmd;
	}
}

int main(int argc,char* argv[]) {
	char* cmd="echo";
	if (argc == 2)
		strcpy(cmd,argv[1]);
	char str;
	if (argc == 1) {
		int counter = 0;
		int rs;
		while ((rs = read(0, &str, sizeof(str))) == 1) {
			int pf[2];
			if (pipe(pf) == -1)
				err(1, "Pipe failed");

			pid_t pid = fork();
			if (pid == -1)
				err(1, "Fork failed");
			if (pid > 0) {
				close(pf[0]);

				if (write(pf[1], &str, 1) != 1)
					err(1, "Writing failure!");
				counter++;
				int words = 0;
				while ((rs = read(0, &str, sizeof(str))) == sizeof(str)) {
					if (str == 0x20 || str==0x0A) {
						words++;
						counter = 0;
						str = ' ';
						if (words >= 2)
							break;
						if (write(pf[1], &str, sizeof(str)) != sizeof(str))
							err(1, "Error while writing to stdout");
						continue;
					}
					counter++;

					if (counter > 4)
						err(1, "The word is larger than 4 symbols!");

					if (write(pf[1], &str, sizeof(str)) != sizeof(str))
						err(1, "Error while writing to stdout!");
				}
				if (rs == -1)
					err(1, "Error while reading from stdin");
				close(pf[1]);
				int status;
				wait(&status);
				if (WEXITSTATUS(status) != 0)
					err(1, "Something went wrong");
			}
			else if (pid == 0) {
				close(pf[1]);
				char buff[9] = { '\0' };
				if (read(pf[0], buff,sizeof(buff)) == -1)
					err(1, "Error while reading!");
				if (execlp("echo","echo",buff,(char*)NULL) == -1)
					err(1, "Execlp failed!");
			}
		}
		if (rs == -1)
			err(1, "Error while reading from stdin");
	}
	else if (argc == 2) {
		int counter = 0;
		int rs;
		while ((rs = read(0, &str, sizeof(str))) == sizeof(str)) {
			int pf[2];
			if (pipe(pf) == -1)
				err(1, "Pipe failed");

			pid_t pid = fork();
			if (pid == -1)
				err(1,"Fork failed");
			if (pid > 0) {
				close(pf[0]);
				if (write(pf[1], &str, sizeof(str)) != sizeof(str))
					err(1, "error while writing");
				counter++;
				int words = 0;

				while ((rs = read(0, &str, sizeof(str))) == sizeof(str)) {
					if (str == ' ' || str == '\n') {
						words++;
						counter = 0;
						str = ' ';
						if (words >= 2)
							break;
						if (write(pf[1], &str, sizeof(str)) != sizeof(str))
							err(1, "error while writing");
						continue;
					}
					counter++;
					if (counter > 4)
						err(1, "The word is larger than 4 symbols");
					if (write(pf[1], &str, 1) != 1)
						err(1, "writing failure");
				}
				if (rs == -1)
					err(1, "Reading failure");
				close (pf[1]);
				int status;
				wait(&status);
				if (WEXITSTATUS(status) != 0)
					err(1, "Something went wrong");
			}
			else if (pid == 0) {
				close(pf[1]);
				char f1[4] = { '\0' };
				char f2[4] = { '\0' };
			}

		}
		if (rs == -1)
			err(1, "error while reading");
	}
}	

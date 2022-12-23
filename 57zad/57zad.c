#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char* argv[]) {
	uint8_t option_d = 0;
	uint8_t option_s = 0;
	
	if (argc == 1)
		errx(1, "invalid arguments");

	if (strcmp(argv[1],"-d") == 0)
		option_d=1;
	else if (strcmp(argv[1],"-s") == 0)
		option_s=1;
	
	if (argc == 2 && option_d)
		errx(1, "invalid arguments");
	if (argc == 2 && option_s)
		errx(1, "invalid arguments");

	const char* str1;

	if (option_d == 1) {
		str1=argv[2];
		int length=strlen(str1);

		int read_sz;
		int write_sz;
		char c;
		while((read_sz=read(0, &c, sizeof(c))) > 0) {
			uint8_t flag = 0;
			for (int i = 0; i < length; ++i) {
				if (str1[i] == c){
					flag = 1;
					break;
				}
			}
			if (flag == 1)
				continue;
			write_sz=write(1, &c, sizeof(c));
			if (write_sz == -1)
				err(1, "unable to write to console");
		}
		if (read_sz == -1)
			err(1, "unable to read from console");
		
		exit(0);
	}

	if (option_s == 1) {
		str1=argv[2];
		char last = NULL;
		int read_sz;
		int write_sz;
		char c;
		int length=strlen(str1);
		while ((read_sz=read(0, &c, sizeof(c))) > 0) {
			uint8_t flag=0;
			if (c == last) {
				for (int i = 0; i < length; ++i) {
					if (c == str1[i]) {
						flag=1;
						break;
					}
				}
			}
			if (flag==1)
				continue;

			write_sz=write(1, &c, sizeof(c));
			if (write_sz == -1)
				err(1, "unable to write to console");
			last=c;
		}
		if (read_sz == -1)
			err(1, "unable to read from console");
		
		exit(0);
	}
	
	const char* str2;	
	if (argc == 3 && option_s==0 && option_d==0) {
		str1=argv[1];
		str2=argv[2];
		if (strlen(str1) != strlen(str2)) {
			errx(1,"STR1 and STR2 should be the same length");
		}
	} else {
		str1=argv[2];
		str2=argv[3];
		if (strlen(str1) != strlen(str2)) {
			errx(1, "STR1 and STR2 should be the same length");
		}
	}
	
	int read_sz;
	int write_sz;
	char c;
	int length=strlen(str1);

	while ((read_sz=read(0, &c, sizeof(c))) > 0) {
		for (int i = 0; i < length; ++i) {
			if (c == str1[i]) {
				c=str2[i];
				break;
			}
		}

			write_sz=write(1, &c, sizeof(c));
			if (write_sz == -1)
			err(1, "Unable to write to console");
	}
	if (read_sz == -1)
		err(1, "unable to read from console");
	exit(0);
}



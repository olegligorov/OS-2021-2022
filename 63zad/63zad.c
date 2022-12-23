#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <err.h>

typedef struct {
	uint32_t n1;
	uint32_t len1;
	uint8_t n2;
	uint8_t len2;
	uint16_t n3;
	uint16_t len3;
	uint64_t n4;
	uint64_t len4;
} af_t;

typedef struct {
	uint16_t n1;
	uint16_t len1;
	uint16_t n2;
	uint16_t len2;
	uint16_t n3;
	uint16_t len3;
	uint16_t n4;
	uint16_t len4;
} affix_t;

int main(int argc, char* argv[]) {
	if (argc != 7)
		errx(1, "Invalid arguments");

	//afix infix uint16_t
	//prefix i crucifixus uint8_t
	//postfix uint32_t
	//suffix uint64_t
	
	//Interval: nacalen el - broj elementi od fajl
	//Komplekt: naredena 4ka od intervali {post,pre,in,suffix}
	//affix serija od komplekti i da bide generiran crucifixus
	
	//15431930 => Komplekt
}

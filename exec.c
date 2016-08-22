/*
 * This file is compiled at runtime by get_macro to get the value of macro
 * get_macro inserts any library required by the macro on top of this code.
 * It also inserts the code '#define MACRO <MACRO>' on top
 * Note: This code will not compile as a standalone. Might need to compile
 * with -w option to suppress warning.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum type_t {
	INTEGER,
	CHARACTER
};

int main(int argc, char *argv[])
{
	char opt;
	char *macro_s = NULL;
	enum type_t type = INTEGER;

	while ((opt = getopt(argc, argv, "sm:l::")) != -1) {
		switch (opt) {
		case 'm':
			macro_s = optarg;
			break;
		case 's':
                	type = CHARACTER;
                	break;
		case 'l':
			/* Ignore if 'l' option */
			break;
		default: /* '?' */
                	fprintf(stderr, "Usage: %s -m MACRO [-s]\n", argv[0]);
                	exit(EXIT_FAILURE);
		}
	}

	if (macro_s == NULL) {
		fprintf(stderr, "Usage: %s -m MACRO [-s]\n", argv[0]);
                exit(EXIT_FAILURE);
	}

	if (type == INTEGER)
		printf("%s = %d\n", macro_s, (int)MACRO);
	else
		printf("%s = %s\n", macro_s, (char *)MACRO);

	return EXIT_SUCCESS;
}

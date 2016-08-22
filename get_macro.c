#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF	2048
#define TEMP_C "/tmp/temp.c"
#define TEMP_OUT "/tmp/temp.o"
#define EXEC_C "exec.c"

static void print_usage(FILE *stream)
{
	fprintf( stream, "Usage: ./get_macro -m macro [-l HEADER FILE] [-s]\n"

		"-m macro:The macro string\n"
		"-l HEADER FILE: Optional header file that contains the macro\n"
		"-s: The value of macro is assumed to be integer."
		"    Supplying this argument indicates that the"\
		"macro is a string.\n"
		"-h: Prints this help message\n");
}

int main(int argc, char *argv[])
{
	char opt;
	char *macro_s = NULL;
	char arg[MAXBUF];
	char command[MAXBUF];
	int libcount = 0, ret;
	char exec_file[MAXBUF];
	char *last_backslash;

	/* exec file is in same hierachy as this executable - hopefully */
	last_backslash = strrchr(argv[0], '/');
	if (last_backslash != NULL) {
		*last_backslash = '\0';
		sprintf(exec_file, "%s/%s", argv[0], EXEC_C);
	} else
		strcpy(exec_file, EXEC_C);

	/* Note: Buffer overflow possible but unliekly, unless intentional */
	arg[0] = '\0';
	sprintf(command, "cp %s %s;", exec_file, TEMP_C);
	while ((opt = getopt(argc, argv, "sm:l:h")) != -1) {
		switch (opt) {
		case 'm':
			macro_s = optarg;
			sprintf(arg, "%s -m %s", arg, optarg);
			break;
		case 's':
			sprintf(arg, "%s -s", arg);
			break;
		case 'l':
			sprintf(command, "%ssed -i '1s/^/#include <%s>\\n/' %s;",
				command, optarg, TEMP_C);
			libcount++;
			break;
		case 'h':
			print_usage(stdout);
			exit(EXIT_SUCCESS);
		default: /* '?' */
                	print_usage(stderr);
			exit(EXIT_FAILURE);
		}
	}

	if (macro_s == NULL) {
		print_usage(stderr);
		exit(EXIT_FAILURE);
	}

	sprintf(command, "%ssed -i '%ds/^/#define MACRO %s\\n/' %s;",
		command, libcount + 1, macro_s, TEMP_C);
	sprintf(command, "%sgcc -w %s -o %s;%s %s;rm -rf %s %s;",
		command, TEMP_C, TEMP_OUT, TEMP_OUT, arg, TEMP_C, TEMP_OUT);


	ret = system(command);
	if (ret != 0) {
		fprintf(stderr, "System call failure\n");
		exit(EXIT_FAILURE);
	}
}

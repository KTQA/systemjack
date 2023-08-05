#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#include "inih/ini.h"

#define VAR_PREFIX "_INI_"
#define VERSION "0.2"

struct inipart {
	char *section;
	char *field;
	char *env;
	bool array;
};
struct inipart iniparts[64];
int    iniparts_idx = 0;

bool dump = false, all = false;

static int handler(
	void* user,
	const char* section,
	const char* field,
	const char* value
) {

	if (dump) {
		printf("section: '%s', field '%s', value '%s'\n", section, field, value);
	} else {

		if (all) {
			printf("%s%s_%s=%s\n", VAR_PREFIX, section, field, value);
		} else {
			for (int i = 0 ; i < iniparts_idx ; i++) {

				if (
					strncmp(iniparts[i].section, section, sizeof(section)) == 0 &&
					strncmp(iniparts[i].field, field, sizeof(field)) == 0
				) {

					if (iniparts[i].env == NULL) {
						printf("%s%s_%s=%s\n", VAR_PREFIX, section, field, value);
					} else {

						if (iniparts[i].array == 1) {
							printf("%s+=(%s)\n", iniparts[i].env, value);
						} else {
							printf("%s=%s\n", iniparts[i].env, value);
						}
					}

				}

			}

		}

	}

	return 1;

}


static void usage (char *name, int status) {

	fprintf(stderr, "%s - export ini files to shell environment variables\n", name);
	fprintf(stderr, "part of the systemjack package, for the purposes of consistency\n");
	fprintf(stderr, "Usage: %s -i <ini_file> [ -a ] [ definition1 definition2 ]\n\n", name);

	fprintf(stderr,
		"Options:\n"
		"  -h     this help message\n"
		"  -i     required, ini file to process\n"
		"  -a     print all entris in ini file\n"
		"\n"
		"If '-a' is not specified, you can pick which variables to export using 'section:field'\n"
		"which will export a variable named '%ssection_field.  If you wish to give it a simpler\n"
		"variable name, you can append a new name after the definition using '=', as in\n"
		"'section:field=myfield', which will define the variable as 'myfield'.  If '-a' is\n"
		"chosen, all variables will be exported using the '%ssection_field' style.\n"
		"\n"
		"To make use of readini, eval the output.\n"
		"\n"
		"Examples:\n"
		"readini -ai /etc/systemjack/main.ini\n"
		"\n"
		"readini -i /etc/systemjack/main.ini :version jack:ports ffmpeg:style=ffstyle\n"
		"\n"
		"Report bugs to <code@ktqa.org>.\n"
		"Website and manual: <https://vis.nu/FIXME>\n",
		VAR_PREFIX, VAR_PREFIX

	);
	exit(status);
}


static void version(char *name) {
	printf(
		"%s %s\n\n"
		"Copyright (C) 2020 Sam Mulvey <code@ktqa.org>\n"
		"This is free software; see the source for copying conditions.  There is NO\n"
		"warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\n",
		name,
		VERSION
	);
	exit(0);

}



int main(int argc, char **argv) {
	bool do_time;
	int o, error;
	char *ini_file = NULL, *section = NULL, *field = NULL,
		 *env = NULL, *tmp = NULL;
	char* tokstate;

	while ( (o = getopt(argc, argv, "tdahvi:")) != -1 ) {

		switch (o) {
			case 'i':
				ini_file = optarg;
				break;

			case 'd':
				dump = true;
				break;

			case 'a':
				all = true;
				break;

			case 'h':
				usage(argv[0], 0);

			case 'v':
				version(argv[0]);
				break;

			case 't':
				do_time = true;
				break;

			default:
				usage(argv[0], 1);
				break;
		}

	}

	// this is one of those things that, if this software gets used,
	// will confuse the fsck out of people.   It's like a saw jammed into
	// a transistor radio.
	if (do_time) {
		int msize = argc + 1;
		struct tm sdtime;
		time_t ts;

		for (int i = optind ; i < argc ; i++) msize += strlen(argv[i]);
		char *str = calloc(msize, sizeof(char));

		msize = 0;
		for (int i = optind ; i < argc ; i++) {
			memcpy(str + msize, argv[i], strlen(argv[i]));
			msize += strlen(argv[i]);
			if (i != argc - 1) str[ msize++ ] = ' ';
		}
		
		if (!strptime(str, "%A %F %T %Z", &sdtime)) {
			fprintf(stderr, "%s: could not parse timestamp\n", argv[0]);
			exit(1);
		} else {
			sdtime.tm_isdst = -1; // hey, find out, yeah?
			ts = mktime(&sdtime);
			printf("%d\n", ts);
		}

		free(str);
		exit(0);
	}

	if (ini_file == NULL) {
		fprintf(stderr, "%s: no ini file specified.\n", argv[0]);
		usage(argv[0], 1);
	}


	if (!all && !dump) {


		if (argc == optind) {
			fprintf(stderr, "%s: no definitions, all not specified.\n", argv[0]);
			usage(argv[0], 1);
		}

		for (int i = optind ; i < argc ; i++) {

			tmp = strtok_r(argv[i], "=", &tokstate);
			env = strtok_r(NULL,    "=", &tokstate);
			section = strtok_r(tmp, ":", &tokstate);
			field   = strtok_r(NULL, ":", &tokstate);

			if (field == NULL) {
				iniparts[ iniparts_idx ].section = "";
				iniparts[ iniparts_idx ].field   = section;
			} else {
				iniparts[ iniparts_idx ].section = section;
				iniparts[ iniparts_idx ].field   = field;
			}

			if (env == NULL) {
				iniparts[ iniparts_idx ].env = "";
			} else {
				if (env != NULL && env[ strlen(env) - 1 ] == '@') {
					iniparts[ iniparts_idx ].array = 1;
					env[ strlen(env) - 1 ] = '\0';
				}
			}

			iniparts[ iniparts_idx ].env     = env;
			iniparts_idx++;

		}

	}


	error = ini_parse(ini_file, handler, NULL);
	if (error < 0) {
		printf("%s: can't read ini file '%s'\n", argv[0], ini_file);
		exit(1);
	} else if (error) {
		printf("%s: ini file error, first error on line %d\n", argv[0], error);
		exit(1);
	}

	exit(0);
}

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "inih/ini.h"

#define VAR_PREFIX "_INI_"

struct inipart {
	char *section;
	char *field;
	char *env;
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
						printf("%s=%s\n", iniparts[i].env, value);
					}

				}

			}

		}

	}

	return 1;

}

void help() {
	printf("help is here\n");
}


int main(int argc, char **argv) {
	int o, error;
	char *ini_file = NULL, *section = NULL, *field = NULL, *env = NULL, *tmp = NULL;

	while ( (o = getopt(argc, argv, "apehdi:")) != -1 ) {

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
				help();
				exit(0);

			default:
				help();
				exit(1);
				break;
		}

	}

	if (ini_file == NULL) {
		help();
		exit(1);
	}


	if (!all && !dump) {


		if (argc == optind) {
			fprintf(stderr, "%s: no definitions, all not specified.\n", argv[0]);
			help();
			exit(1);
		}

		for (int i = optind ; i < argc ; i++) {

			tmp = strtok(argv[i], "=");
			env = strtok(NULL,    "=");
			section = strtok(tmp, ":");
			field   = strtok(NULL, ":");

			if (field == NULL) {
				iniparts[ iniparts_idx ].section = "";
				iniparts[ iniparts_idx ].field   = section;
			} else {
				iniparts[ iniparts_idx ].section = section;
				iniparts[ iniparts_idx ].field   = field;
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

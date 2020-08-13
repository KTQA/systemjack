#define _XOPEN_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// "Wed 2020-08-12 18:03:43 PDT"
// "%A %F %T %Z"
int main(int argc, char **argv) {


	int msize = argc + 1;
	struct tm sdtime;
	time_t ts;

	for (int i = 1 ; i < argc ; i++) msize += strlen(argv[i]);
	char *str = calloc(msize, sizeof(char));

	msize = 0;
	for (int i = 1 ; i < argc ; i++) {
		memcpy(str + msize, argv[i], strlen(argv[i]));
		msize += strlen(argv[i]);
		if (i != argc - 1) str[ msize++ ] = ' ';
	}
	
	if (!strptime(str, "%A %F %T %Z", &sdtime)) {
		fprintf(stderr, "%s: could not parse string\n", argv[0]);
		exit(1);
	} else {

		sdtime.tm_isdst = -1; // hey, find out, yeah?
		ts = mktime(&sdtime);
		printf("%d\n", ts);

	}

	free(str);



}

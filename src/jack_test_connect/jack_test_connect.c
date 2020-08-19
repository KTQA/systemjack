/**
 *
 * stripped down from jack_multithru
 *
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jack/jack.h>
#include <stdbool.h>

jack_client_t *client;


void help (int code) 
{
	fprintf(stderr, "Usage: jack_test_connect [options]\n");
	fprintf(stderr, "Connect and activate a JACK client, insuring that JACK is running.\n");
	fprintf(stderr, "        -s <name>   Connect to jack server named <name>\n");
	fprintf(stderr, "        -h          this help\n");
	fprintf(stderr, "Currently part of systemjack.\n");

	exit(code);
}



int main (int argc, char *argv[]) {
	int i;
    const char *server_name = NULL;
    const char *client_name = "jack_test_connect";
	jack_status_t status;


	while ((i = getopt(argc, argv, "s:h")) != -1) {
		switch (i) {
			case 's':
				server_name = optarg;
				break;

			case 'h':
				help(0);
				break;

			default:
				help(1);
				break;
		}
	}

	if (server_name != NULL) fprintf(stderr, "%s: server name '%s'\n", argv[0], server_name);
    client = jack_client_open(client_name, JackNoStartServer, &status, server_name);

	if (client == NULL) {
		fprintf(stderr, "%s: could not connect\n", argv[0]);
		exit(1);
	} else {

		if (jack_activate(client)) {
			fprintf(stderr, "%s: could not activate\n", argv[0]);
			exit(1);
		} else {
			jack_client_close(client);
			exit(0);
		}

	}	
}

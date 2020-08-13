/** jack_multithru
 *
 * Adapted from jack_thru in the jack2 source to specify number of channels
 * and avoid autoconnect.
 *
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <jack/jack.h>
#include <stdbool.h>

#define MAX_CHANNELS 32

jack_port_t **input_ports;
jack_port_t **output_ports;
jack_client_t *client;

int multithru_channels = 2;

bool meterbridge_compat = false;

static void signal_handler ( int sig )
{
    jack_client_close ( client );
    fprintf ( stderr, "signal received, exiting ...\n" );
    exit ( 0 );
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client follows a simple rule: when the JACK transport is
 * running, copy the input port to the output.  When it stops, exit.
 */

int
process ( jack_nframes_t nframes, void *arg )
{
    int i;
    jack_default_audio_sample_t *in, *out;
    for ( i = 0; i < multithru_channels; i++ )
    {
        in = jack_port_get_buffer ( input_ports[i], nframes );
        out = jack_port_get_buffer ( output_ports[i], nframes );
        memcpy ( out, in, nframes * sizeof ( jack_default_audio_sample_t ) );
    }
    return 0;
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void
jack_shutdown ( void *arg )
{
    free ( input_ports );
    free ( output_ports );
    exit ( 1 );
}



void 
help (int code) 
{
	fprintf(stderr, "Usage: jack_multithru [options]\n");
	fprintf(stderr, "Simple client that passes audio from an arbitrary number of in ports to \n");
	fprintf(stderr, "corresponding out ports.  For when meterkeeper is too much and aliases\n");
	fprintf(stderr, "aren't enough.\n");
	fprintf(stderr, "        -s <name>   Connect to jack server named <name>\n");
	fprintf(stderr, "        -n <name>   Name this client <name>\n");
	fprintf(stderr, "        -c <num>    Create 1-%d channels\n", MAX_CHANNELS);
	fprintf(stderr, "        -m          Use meterbridge naming conventions\n");
	fprintf(stderr, "Currently part of systemjack, adapted from jack_thru.\n");

	exit(code);
}

int
main ( int argc, char *argv[] )
{
    int i;
    const char **ports;
    const char *client_name = "jack_multithru";
    const char *server_name = NULL;
    jack_options_t options = JackNullOption;
    jack_status_t status;

	/* do args */

	while ((i = getopt(argc, argv, "mn:c:s:h")) != -1)
	{
		switch (i) {
			case 'n':
				client_name = optarg;
				break;

			case 'c':
				multithru_channels = atoi(optarg);
				break;

			case 's':
				server_name = optarg;
				break;

			case 'h':
				help(0);
				break;

			case 'm':
				meterbridge_compat = true;


		}

	}

	if (multithru_channels == 0 || multithru_channels > MAX_CHANNELS) {
		fprintf(stderr, "bad channel count\n");
		help(-1);
	}
	if (server_name != NULL) fprintf(stderr, "jack_multithru: server name '%s'\n", server_name);
	fprintf(stderr, "jack_multithru: name '%s', channels %d\n", client_name, multithru_channels);
	



    /* open a client connection to the JACK server */

    client = jack_client_open ( client_name, options, &status, server_name );
    if ( client == NULL )
    {
        fprintf ( stderr, "jack_client_open() failed, "
                  "status = 0x%2.0x\n", status );
        if ( status & JackServerFailed )
        {
            fprintf ( stderr, "Unable to connect to JACK server\n" );
        }
        exit ( 1 );
    }
    if ( status & JackServerStarted )
    {
        fprintf ( stderr, "JACK server started\n" );
    }
    if ( status & JackNameNotUnique )
    {
        client_name = jack_get_client_name ( client );
        fprintf ( stderr, "unique name `%s' assigned\n", client_name );
    }

    /* tell the JACK server to call `process()' whenever
       there is work to be done.
    */

    jack_set_process_callback ( client, process, 0 );

    /* tell the JACK server to call `jack_shutdown()' if
       it ever shuts down, either entirely, or if it
       just decides to stop calling us.
    */

    jack_on_shutdown ( client, jack_shutdown, 0 );

    /* create two ports pairs*/
    input_ports = ( jack_port_t** ) calloc ( multithru_channels, sizeof ( jack_port_t* ) );
    output_ports = ( jack_port_t** ) calloc ( multithru_channels, sizeof ( jack_port_t* ) );

    char port_name[16];
    for ( i = 0; i < multithru_channels; i++ )
    {

		if (meterbridge_compat)
		{
        	sprintf ( port_name, "meter_%d", i + 1 );
		} else {
        	sprintf ( port_name, "input_%d", i + 1 );
		}
        input_ports[i] = jack_port_register ( client, port_name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 );

        if (meterbridge_compat)
		{
			sprintf( port_name, "monitor_%d", i + 1 );
		} else {
			sprintf ( port_name, "output_%d", i + 1 );
		}
        output_ports[i] = jack_port_register ( client, port_name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 );


        if ( ( input_ports[i] == NULL ) || ( output_ports[i] == NULL ) )
        {
            fprintf ( stderr, "no more JACK ports available\n" );
            exit ( 1 );
        }
    }

    /* Tell the JACK server that we are ready to roll.  Our
     * process() callback will start running now. */

    if ( jack_activate ( client ) )
    {
        fprintf ( stderr, "cannot activate client" );
        exit ( 1 );
    }

	// For my use, I don't want it to connect the ports, so I pulled that code out.


    /* install a signal handler to properly quits jack client */
    signal ( SIGQUIT, signal_handler );
    signal ( SIGTERM, signal_handler );
    signal ( SIGHUP, signal_handler );
    signal ( SIGINT, signal_handler );

    /* keep running until the transport stops */

    while (1)
    {
        sleep ( 1 );
    }

    jack_client_close ( client );
    exit ( 0 );
}


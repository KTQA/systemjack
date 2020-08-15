## This is for any environment variables you'd like to set up
## for your systemjack environment.

## Stuff we need goes here:

## If you're running a GUI, you'll want to set this variable.
## This is a hack and will be updated.
export DISPLAY=":0"

## Needed for DBUS jack interface, if you use that sort of thing.
export DBUS_SESSION_BUS_ADDRESS=unix:path=/run/dbus/system_bus_socket 

## Jack environment variable to prevent jack clients from
## starting up their own copy of jackd if they can't find the jack
## server.   You want this.
export JACK_NO_START_SERVER=1 

## an extra function for failure emails, if needed.
## systemjack-email will call this function
## takes the system what failed as an argument.

email_extra() {
	:; # delete this line if this function needs to do something
}



## Stuff we need ends here.  Cusomization begins here.


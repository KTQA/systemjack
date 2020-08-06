#!/usr/bin/env bash

#echo "functions.sh loaded"

READINI=${READINI:-$(which readini)}

die() {
	echo -n "systemjack error: "
	echo $1
	exit 1
}

warn() {
	echo -n "systemjack warning: "
	echo $1
}

say() {
	echo -n "systemjack: "
	echo $1
}

is_true() {
	if [ "$1" == "true" ] || [ "$1" == "1" ]; then
		return 0
	fi
	return 1
}

# sometimes, the opposite of false is not true, but set
is_false() {
	if [ "$1" == "false" ] || [ "$1" == "" ] || [ "$1" == "0" ]; then
		return 0
	fi
	return 1
}

if [ ! -x $READINI ]; then
	die "readini is not right"
fi

#if [ ! -f /etc/systemd/system/jackd.service.d/override.conf ]; then
	#die "systemjack-setup not run, please run that first."
#fi

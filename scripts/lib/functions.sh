#!/usr/bin/env bash

READINI=${READINI:-$(which readini)}

die() {
	echo -n "systemjack error: " >&2
	echo $1
	exit 1
}

warn() {
	echo -n "systemjack warning: " >&2
	echo $1
}

say() {
	echo -n "systemjack: " >&2
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

do_in_urxvt() {

	local cmd=$1
	local title=$2
	local hide=$3
	local extra=""
	local urxvt=$(which urxvt)

	title=${title:-"SystemJack Window"}
	hide=${hide:-0}

	if [ -z "$cmd" ]; then 
		die "do_in_urxvt -- no command specfified"
	fi

	if [ "$hide" == "1" ]; then

		local xdotool=$(which xdotool)
		if [ ! -x $xdotool ]; then
			warn "xdotool not installed and hide requested, not minimizing"
		fi
		extra="$xdotool windowminimize \$WINDOWID &&"

	fi




	if [ -x $urxvt ]; then 
		exec $urxvt -T "${title}" \
			-e $(which bash) -c "$extra $1"
	else
		die "no urxvt binary available"
	fi


}

do_in_screen() {

	local cmd=$1
	local name=$2
	local screen=$(which screen)

	name=${name:-"SystemJack"}

	if [ -z "$cmd" ]; then 
		die "do_in_screen -- no command specfified"
	fi


	if [ -x $screen ]; then 
		exec $screen -DmS "$name" -- $cmd
	else
		die "no screen binary available"
	fi
}


global_gui() {

	case $SYSTEMJACK_GUI in
		true)
			echo $1
			
			;;

		false)
			echo $2
			;;

		*)
			echo $3
			;;

	esac

}





# startup sanity checks
if [ ! -x $READINI ]; then
	die "readini is not right"
fi

if [ "$(basename $0)" != "systemjack-setup" ]; then

	# check if systemjack-setup has been completed
	if [ ! -f /etc/systemd/system/jackd.service.d/override.conf ]; then
		die "systemjack-setup not run, please run that first."
	fi

	# Get global GUI config, unset DISPLAY if false.
	eval `$READINI -i ${INI_FILES}/main.ini setup:gui=SYSTEMJACK_GUI`
	if [ "$SYSTEMJACK_GUI" == "false" ]; then
		unset DISPLAY
	fi

fi

## Services that systemjack has to manage.  If other packages have
## service files that depend on systemjack, they need to be added to
## this array.
SYSTEMJACK_SERVICES=(
	"aj-snapshot.service"
	"calf.service"
	"capture@.service"
	"ffmpeg@.service"
	"jackd.service"
	"patchpanel@.service"
	"non-mixer.service"
	"silentjack.service"
	"mumble.service"
	"sonobus.service"
)

shopt -s nullglob
include_files=(${SCRIPT_DIR}/extra.d/*.sh)
shopt -u nullglob
if [ ${#include_files[@]} -gt 0 ]; then
	for file in "${include_files[@]}"; do
		. $file
	done
fi


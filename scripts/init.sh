#!/usr/bin/env bash

# set directories for scripts.
INI_FILES=${INI:-"/etc/systemjack"}
SCRIPT_DIR=${SCRIPTS:-"/usr/lib/systemjack"}

# load functions and environment
. "${SCRIPT_DIR}/functions.sh"
. "${INI_FILES}/env.sh"

echo "DISPLAY: ${DISPLAY}"

# and, go!


eval `$READINI -i "${INI_FILES}/main.ini" setup:pci=pci setup:rtc0=rtc setup:hpet=hpet setup:tmp=tmpdir`

# Giving the sound card priority on the PCI bus
if ! is_false $pci; then
	say "Setting PCI priority for device ${pci}"
	setpci -v -d *:* latency_timer=b0
	setpci -v -s ${pci} latency_timer=ff
fi

# Setting usable freq for clocks
if ! is_false $rtc; then
	say "Setting RTC0"
	if [ -f /sys/class/rtc/rtc0/max_user_freq ]; then
		echo $rtc > /sys/class/rtc/rtc0/max_user_freq
	else
		warn "/sys/class/rtc/rtc0/max_user_freq does not exist"
	fi
fi


if ! is_false $hpet; then
	say "Setting HPET"
	if [ -f /proc/sys/dev/hpet/max-user-freq ]; then
		echo $hpet > /proc/sys/dev/hpet/max-user-freq
	else
		warn "/proc/sys/dev/hpet/max-user-freq does not exist"
	fi
fi

# bit of a hack here, waiting to hear from the UI to start graphical apps
# other apps (but not jack) depends on this script to complete before launching
if [ ! -z ${DISPLAY+x} ]; then
	say "Waiting for GUI to start"
	while [ ! -f "${tmpdir}/.systemjack_gui_started" ]; do sleep 1; done
	say "GUI signaled start, completing."
fi

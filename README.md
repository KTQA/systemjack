systemjack - jackd and tools out of systemd
==========================

This is a pile of stuff that lets me run JACK apps right out of boot in systemd.   Mostly for Arch Linux, but there is nothing really that arch specific in it.

Very, very fast overview
---------------------------

systemjack parses INI files in ``/etc/systemjack`` to configure daemons and subsystems and launch them out of systemd.  The config files have some documentation in them, but full documentation is forthcoming.

First Four Steps:
- Install package
- Edit at least /etc/systemjack/main.ini
- Run ``/usr/bin/systemjack-setup``
- enable systemjack-init and jackd 

Supported Apps
-----------------

- [JACK audio](https://jackaudio.org/), obviously.
- [aj-snapshot](https://github.com/sreimers/aj-snapshot), a method for maintaining connections inside JACK's patchbay
- [jack_capture](https://github.com/kmatheussen/jack_capture), a method for recording audio from JACK in a multitude of formats
- [ffmpeg](https://ffmpeg.org/), for a lot of reasons, but notably for streaming to icecast
- [mumble](https://www.mumble.info/), for secure, low latency live audio transport with chat and possible talkback
- [non-mixer](http://non.tuxfamily.org/wiki/Non%20Mixer), a software mixer
- [meterbridge](http://plugin.org.uk/meterbridge/), a method for displaying a meter
- [silentjack](https://www.aelius.com/njh/silentjack/), a silence (or dead air) detector
- [calf](http://calf-studio-gear.org/), a collection of live effects

There is also a method to add more applications to systemjack.  Documentation is (unsurprisingly) forthcoming.





License
--------

LIcense is going to be something AGPL3 like, but listeners are construed as users until I find a better way to say it.
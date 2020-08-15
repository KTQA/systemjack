PREFIX=/usr
ETC_PREFIX=

SRCTARGETS := all clean
$(SRCTARGETS):
	$(MAKE) -C src/ $(MAKECMDGOALS)

.PHONY: install_services
install_services:
	mkdir -p $(DESTDIR)$(PREFIX)/lib/systemd/system
	install -m644 services/*service $(DESTDIR)$(PREFIX)/lib/systemd/system

.PHONY: install_libexec
install_libexec:
	mkdir -p $(DESTDIR)$(PREFIX)/lib/systemjack
	install -m755 scripts/lib/* $(DESTDIR)$(PREFIX)/lib/systemjack

.PHONY: install_binscripts
install_binscripts:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 scripts/bin/* $(DESTDIR)$(PREFIX)/bin

.PHONY: install_etc
install_etc:
	mkdir -p $(DESTDIR)$(ETC_PREFIX)/etc/systemjack/env.d
	install -m644 etc/*.ini $(DESTDIR)$(ETC_PREFIX)/etc/systemjack

.PHONY: install_share
install_share:
	mkdir -p $(DESTDIR)$(PREFIX)/share/systemjack/templates
	#install -m644 share/* $(DESTDIR)$(PREFIX)/share/systemjack -- nothing yet
	install -m644 share/templates/* $(DESTDIR)$(PREFIX)/share/systemjack/templates


install: install_services install_libexec install_binscripts install_etc install_share
	PREFIX=$(PREFIX) DESTDIR=$(DESTDIR) $(MAKE) -C src/ install



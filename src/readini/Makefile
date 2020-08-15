CC=gcc
PREFIX=/usr

default: readini

.PHONY: clean
clean:
	rm readini

readini:
	$(CC) -o readini readini.c inih/ini.c -DINI_ALLOW_NO_VALUE=1

.PHONY: install
install: readini
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/readini

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/readini

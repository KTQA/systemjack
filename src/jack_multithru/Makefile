CC=gcc
LIBS=-ljack
PREFIX=/usr


default: jack_multithru

.PHONY: clean
clean:
	rm jack_multithru

jack_multithru:
	$(CC) -o jack_multithru $(LIBS) jack_multithru.c


.PHONY: install
install: jack_multithru
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/jack_multithru

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/jack_multithru


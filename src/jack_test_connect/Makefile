CC=gcc
LIBS=-ljack
PREFIX=/usr


default: jack_test_connect

.PHONY: clean
clean:
	rm jack_test_connect

jack_test_connect:
	$(CC) -o jack_test_connect $(LIBS) jack_test_connect.c


.PHONY: install
install: jack_test_connect
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/jack_test_connect

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/jack_test_connect


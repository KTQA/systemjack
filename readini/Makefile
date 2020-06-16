CC=gcc

default: readini

.PHONY: clean
clean:
	rm readini

readini:
	$(CC) -o readini readini.c inih/ini.c -DINI_ALLOW_NO_VALUE=1


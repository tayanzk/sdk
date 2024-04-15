CFLAGS=-I. -Wall -Werror -Wextra -ggdb

all: sdk

%.o: sdk/%.c sdk/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

sdk: arena.o buf.o map.o hash.o bus.o fs.o time.o
	ar rcs libsdk.a arena.o buf.o map.o hash.o bus.o fs.o time.o

.PHONY: install clean

install:
	cp -rf sdk/ /usr/local/include/
	cp -f libsdk.a /usr/local/lib/

clean:
	rm -f *.o *.a

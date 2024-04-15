CFLAGS=-I.

all: sdk

%.o: sdk/%.c sdk/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

sdk: arena.o buf.o map.o hash.o bus.o fs.o
	ar rcs libsdk.a arena.o buf.o map.o hash.o bus.o fs.o

install:
	cp -r sdk/ /usr/local/include/sdk
	cp libsdk.a /usr/local/lib/

.PHONY: clean

clean:
	rm -f *.o *.a

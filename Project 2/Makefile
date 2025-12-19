MAKEFLAGS += -s

CFLAGS = -g -Wall -Werror

TAR = tar
ifeq ($(shell uname -s), Darwin)
    TAR = gtar
endif

SRC = lib_tar.c tests.c
OBJ = lib_tar.o tests.o

all: tests

lib_tar.o: lib_tar.c lib_tar.h
	$(CC) $(CFLAGS) -c lib_tar.c

tests.o: tests.c lib_tar.h
	$(CC) $(CFLAGS) -c tests.c

tests: $(OBJ)
	$(CC) $(CFLAGS) -o tests $(OBJ)

test: tests
	./tests test_archive.tar

clean:
	rm -f lib_tar.o tests.o tests soumission.tar

submit: all
	$(TAR) --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c *.h *.c Makefile > soumission.tar
MAKEFLAGS += -s #on redéfinit make pour ne pas avoir toutes les lignes dans le terminal à l'exécution

CC = gcc

UNAME_M := $(shell uname -m)
UNAME_S := $(shell uname -s)

CFLAGS = -w -pthread -I include

#on rajoute '-arch x86_64' si sur macOs pour forcer l'exécution dans ce type-là
CFLAGS := $(if $(filter arm64,$(UNAME_M)), $(CFLAGS) -arch x86_64, $(CFLAGS))

###PARTIE SOURCES###

#IMPLEM DE BASES
SRC1 = src/philosoph.c #1.1
SRC2 = src/prod_cons.c #1.2
SRC3 = src/reader_writer.c #1.3

SRC4 = src/test_and_set/test_and_set.c #2.1
SRC5 = src/test_and_test_and_set/test_and_test_and_set.c #2.2

#PARTIE TEST_AND_SET
SRC6 = src/test_and_set/test_and_set_semaphore.c
SRC7 = src/test_and_set/test_and_set_philosoph.c 
SRC8 = src/test_and_set/test_and_set_prod_cons.c
SRC9 = src/test_and_set/test_and_set_reader_writer.c
SRC14 = src/test_and_set/test_and_set_main.c

#PARTIE TEST_AND_TEST_AND_SET
SRC10 = src/test_and_test_and_set/test_and_test_and_set_semaphore.c
SRC11 = src/test_and_test_and_set/test_and_test_and_set_philosoph.c
SRC12 = src/test_and_test_and_set/test_and_test_and_set_prod_cons.c
SRC13 = src/test_and_test_and_set/test_and_test_and_set_reader_writer.c
SRC15 = src/test_and_test_and_set/test_and_test_and_set_main.c



###PARTIE OBJETS###

#IMPLEM DE BASES
OBJ1 = src/philosoph.o
OBJ2 = src/prod_cons.o
OBJ3 = src/reader_writer.o


OBJ4 = src/test_and_set/test_and_set.o
OBJ5 = src/test_and_test_and_set/test_and_test_and_set.o

#PARTIE TEST_AND_SET
OBJ6 = src/test_and_set/test_and_set_semaphore.o
OBJ7 = src/test_and_set/test_and_set_philosoph.o
OBJ8 = src/test_and_set/test_and_set_prod_cons.o
OBJ9 = src/test_and_set/test_and_set_reader_writer.o
OBJ14 = src/test_and_set/test_and_set_main.o

#PARTIE TEST_AND_TEST_AND_SET
OBJ10 = src/test_and_test_and_set/test_and_test_and_set_semaphore.o
OBJ11 = src/test_and_test_and_set/test_and_test_and_set_philosoph.o
OBJ12 = src/test_and_test_and_set/test_and_test_and_set_prod_cons.o
OBJ13 = src/test_and_test_and_set/test_and_test_and_set_reader_writer.o
OBJ15 = src/test_and_test_and_set/test_and_test_and_set_main.o



###PARTIE EXECUTABLE###

#IMPLEM DE BASES
EXEC1 = src/philosoph
EXEC2 = src/prod_cons
EXEC3 = src/reader_writer

#PARTIE TEST_AND_SET
EXEC7 = src/test_and_set/test_and_set_philosoph
EXEC8 = src/test_and_set/test_and_set_prod_cons
EXEC9 = src/test_and_set/test_and_set_reader_writer
EXEC14 = src/test_and_set/test_and_set_main

#PARTIE TEST_AND_TEST_AND_SET
EXEC11 = src/test_and_test_and_set/test_and_test_and_set_philosoph
EXEC12 = src/test_and_test_and_set/test_and_test_and_set_prod_cons
EXEC13 = src/test_and_test_and_set/test_and_test_and_set_reader_writer
EXEC15 = src/test_and_test_and_set/test_and_test_and_set_main



all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC7) $(EXEC8) $(EXEC9) $(EXEC11) $(EXEC12) $(EXEC13) $(EXEC14) $(EXEC15)

#PHILOSOPH
$(EXEC1): $(OBJ1)
	$(CC) -o $(EXEC1) $(OBJ1) $(CFLAGS)

#PROD_CONS
$(EXEC2): $(OBJ2)
	$(CC) -o $(EXEC2) $(OBJ2) $(CFLAGS)

#READER_WRITER
$(EXEC3): $(OBJ3)
	$(CC) -o $(EXEC3) $(OBJ3) $(CFLAGS)

#NEW_PHILOSOPH - TEST_AND_SET
$(EXEC7): $(OBJ7) $(OBJ6) $(OBJ4)
	$(CC) -o $(EXEC7) $(OBJ7) $(OBJ6) $(OBJ4) $(CFLAGS)

#NEW_PROD_CONS - TEST_AND_SET
$(EXEC8): $(OBJ8) $(OBJ6) $(OBJ4)
	$(CC) -o $(EXEC8) $(OBJ8) $(OBJ6) $(OBJ4) $(CFLAGS)

#NEW_READER_WRITER - TEST_AND_SET
$(EXEC9): $(OBJ9) $(OBJ6) $(OBJ4)
	$(CC) -o $(EXEC9) $(OBJ9) $(OBJ6) $(OBJ4) $(CFLAGS)

#TEST_AND_SET_MAIN - TEST_AND_SET
$(EXEC14): $(OBJ14) $(OBJ4)
	$(CC) -o $(EXEC14) $(OBJ14) $(OBJ4) $(CFLAGS)

#NEW_PHILOSOPH - TEST_AND_TEST_AND_SET
$(EXEC11): $(OBJ11) $(OBJ10) $(OBJ5)
	$(CC) -o $(EXEC11) $(OBJ11) $(OBJ10) $(OBJ5) $(CFLAGS)

#NEW_PROD_CONS - TEST_AND_TEST_AND_SET
$(EXEC12): $(OBJ12) $(OBJ10) $(OBJ5)
	$(CC) -o $(EXEC12) $(OBJ12) $(OBJ10) $(OBJ5) $(CFLAGS)

#NEW_READER_WRITER - TEST_AND_TEST_AND_SET
$(EXEC13): $(OBJ13) $(OBJ10) $(OBJ5)
	$(CC) -o $(EXEC13) $(OBJ13) $(OBJ10) $(OBJ5) $(CFLAGS)

#TEST_AND_TEST_AND_SET_MAIN - TEST_AND_TEST_AND_SET
$(EXEC15): $(OBJ15) $(OBJ5)
	$(CC) -o $(EXEC15) $(OBJ15) $(OBJ5) $(CFLAGS)



$(OBJ1): $(SRC1)
	$(CC) -c $(SRC1) -o $(OBJ1) $(CFLAGS)

$(OBJ2): $(SRC2)
	$(CC) -c $(SRC2) -o $(OBJ2) $(CFLAGS)

$(OBJ3): $(SRC3)
	$(CC) -c $(SRC3) -o $(OBJ3) $(CFLAGS)

$(OBJ4): $(SRC4)
	$(CC) -c $(SRC4) -o $(OBJ4) $(CFLAGS)

$(OBJ5): $(SRC5)
	$(CC) -c $(SRC5) -o $(OBJ5) $(CFLAGS)

$(OBJ7): $(SRC7)
	$(CC) -c $(SRC7) -o $(OBJ7) $(CFLAGS)

$(OBJ8): $(SRC8)
	$(CC) -c $(SRC8) -o $(OBJ8) $(CFLAGS)

$(OBJ9): $(SRC9)
	$(CC) -c $(SRC9) -o $(OBJ9) $(CFLAGS)

$(OBJ11): $(SRC11)
	$(CC) -c $(SRC11) -o $(OBJ11) $(CFLAGS)

$(OBJ12): $(SRC12)
	$(CC) -c $(SRC12) -o $(OBJ12) $(CFLAGS)

$(OBJ13): $(SRC13)
	$(CC) -c $(SRC13) -o $(OBJ13) $(CFLAGS)

$(OBJ14): $(SRC14)
	$(CC) -c $(SRC14) -o $(OBJ14) $(CFLAGS)

$(OBJ15): $(SRC15)
	$(CC) -c $(SRC15) -o $(OBJ15) $(CFLAGS)

philosoph: $(EXEC1)
	./$(EXEC1) $(N)

prod_cons: $(EXEC2)
	./$(EXEC2) $(PROD) $(CONS)

reader_writer: $(EXEC3)
	./$(EXEC3) $(WRITER) $(READER)

test_and_set_philosoph: $(EXEC7)
	./$(EXEC7) $(N)

test_and_set_prod_cons: $(EXEC8)
	./$(EXEC8) $(PROD) $(CONS)

test_and_set_reader_writer: $(EXEC9)
	./$(EXEC9) $(READER) $(WRITER)

test_and_set_main: $(EXEC14)
	./$(EXEC14) $(N)

test_and_test_and_set_philosoph: $(EXEC11)
	./$(EXEC11) $(N)

test_and_test_and_set_prod_cons: $(EXEC12)
	./$(EXEC12) $(PROD) $(CONS)

test_and_test_and_set_reader_writer: $(EXEC13)
	./$(EXEC13) $(READER) $(WRITER)

test_and_test_and_set_main: $(EXEC15)
	./$(EXEC15) $(N)

clean:
	rm -f src/*.o src/test_and_set/*.o src/test_and_test_and_set/*.o src/philosoph src/prod_cons src/reader_writer src/test_and_set/test_and_set src/test_and_test_and_set/test_and_test_and_set src/test_and_set/test_and_set_philosoph src/test_and_set/test_and_set_prod_cons src/test_and_set/test_and_set_reader_writer src/test_and_set/test_and_set_main src/test_and_test_and_set/test_and_test_and_set_philosoph src/test_and_test_and_set/test_and_test_and_set_prod_cons src/test_and_test_and_set/test_and_test_and_set_reader_writer src/test_and_test_and_set/test_and_test_and_set_main

.PHONY: all philosoph prod_cons reader_writer test_and_set my_semaphore test_and_set_main test_and_test_and_set new_philosoph new_prod_cons new_reader_writer test_and_test_and_set_main clean

run:
	chmod +x experiments.sh # S'assure que le script est exécutable
	./experiments.sh

archive:
	zip -r archive.zip . -x@exclude.txt
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -fsanitize=address

all: generate extract

generate: ./src/generate.c ean8_utils.o pbm_utils.o
	$(CC) $(CFLAGS) -o generate ./src/generate.c ean8_utils.o pbm_utils.o

extract: ./src/extract.c ean8_utils.o pbm_utils.o
	$(CC) $(CFLAGS) -o extract ./src/extract.c ean8_utils.o pbm_utils.o

ean8_utils.o: ./src/ean8_utils.c ./src/ean8_utils.h
	$(CC) $(CFLAGS) -c ./src/ean8_utils.c

pbm_utils.o: ./src/pbm_utils.c ./src/pbm_utils.h
	$(CC) $(CFLAGS) -c ./src/pbm_utils.c

clean:
	rm -f *.o *.pbm generate extract

CC = gcc
CFLAGS = -Wall

default: serwer klient

serwer: i_s.c
	gcc i_s.c -o serwer

klient: i_k.c
	gcc i_k.c -o klient

clean:
	-rm -f *.c

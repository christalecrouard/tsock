CC=gcc
CFLAGS=-W

make: main.c liste.c SoPu.c bal.c
	$(CC) $(CFLAGS) bal.c SoPu.c liste.c main.c -o tsock

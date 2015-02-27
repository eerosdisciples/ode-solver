# Make tests

CC=gcc
CFLAGS=-Wall -Wextra -c -g -O0 -I../include
LD=gcc
LFLAGS=

ODOMAIN=domain.o ../domain.o ../readfile.o

all: domain

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

../%.o: ../%.c
	$(CC) $(CFLAGS) -o $@ $<

domain: $(ODOMAIN)
	$(LD) $(LFLAGS) -o $@ $(ODOMAIN)

clean:
	rm *.o
	rm domain

# Make tests

CC=gcc
CFLAGS=-Wall -Wextra -c -g -O0 -I../include
LD=gcc
LFLAGS=

ODOMAIN=domain.o ../domain.o ../readfile.o
OMAGFIELD=magnetic_field.o ../magnetic_field.o ../readfile.o ../vector.o

all: domain magnetic_field

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

../%.o: ../%.c
	$(CC) $(CFLAGS) -o $@ $<

domain: $(ODOMAIN)
	$(LD) $(LFLAGS) -o $@ $(ODOMAIN)

magnetic_field: $(OMAGFIELD)
	$(LD) $(LFLAGS) -o $@ $(OMAGFIELD)


clean:
	rm *.o
	rm domain

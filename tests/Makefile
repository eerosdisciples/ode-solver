# Make tests

CC=gcc
CFLAGS=-Wall -Wextra -c -g -O0 -I../include
LD=gcc
LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas -lm

ODOMAIN=domain.o ../domain.o ../readfile.o
OMAGFIELD=magnetic_field.o ../interp2.o ../magnetic_field.o ../readfile.o ../vector.o
OODE=ode.o ../ode.o ../vector.o ../equation.o

all: domain magnetic_field ode

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

../%.o: ../%.c
	$(CC) $(CFLAGS) -o $@ $<

domain: $(ODOMAIN)
	$(LD) $(LFLAGS) -o $@ $(ODOMAIN)

magnetic_field: $(OMAGFIELD)
	$(LD) $(OMAGFIELD) $(LFLAGS) -o $@ 

ode: $(OODE)
	$(LD) $(OODE) $(LFLAGS) -o $@

clean:
	rm *.o
	rm domain

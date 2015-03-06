# Make tests

CC=gcc
CFLAGS=-Wall -Wextra -c -g -O0 -I../include
LD=gcc
LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas -lm

OCTSV=ctsv.o ../ctsv.o ../vector.o
ODOMAIN=domain.o ../domain.o ../readfile.o
OMAGFIELD=magnetic_field.o ../interp2.o ../magnetic_field.o ../readfile.o ../vector.o
OINPUT=input.o ../arguments.o ../input.o ../readfile.o
OODE=../ctsv.o ode.o ../ode.o ../vector.o ../equation.o

all: ctsv domain magnetic_field input ode

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

../%.o: ../%.c
	$(CC) $(CFLAGS) -o $@ $<

ctsv: $(OCTSV)
	$(LD) $(OCTSV) $(LFLAGS) -o $@ 
domain: $(ODOMAIN)
	$(LD) $(ODOMAIN) $(LFLAGS) -o $@ 

magnetic_field: $(OMAGFIELD)
	$(LD) $(OMAGFIELD) $(LFLAGS) -o $@ 

input: $(OINPUT)
	$(LD) $(OINPUT) $(LFLAGS) -o $@

ode: $(OODE)
	$(LD) $(OODE) $(LFLAGS) -o $@

clean:
	rm *.o
	rm domain

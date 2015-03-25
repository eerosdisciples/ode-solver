# Make tests

CC=gcc
CFLAGS=-Wall -Wextra -c -g -O0 -I../include -pg
LD=gcc
LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas -lm

OCTSV=ctsv.o ../ctsv.o ../vector.o
ODOMAIN=domain.o ../domain.o ../readfile.o
OMAGFIELD=magnetic_field.o ../interp2.o ../magnetic_field.o ../readfile.o ../vector.o
OINPUT=input.o ../arguments.o ../input.o ../readfile.o
OODE=../ctsv.o ode.o ../rkf45.o ../vector.o ../equation_predprey.o ../magnetic_field.o ../interp2.o ../readfile.o
OFFG=../fieldFineGrid.o ../magnetic_field.o ../interp2.o ../readfile.o ../vector.o
ODIFF=diff.o ../diff.o ../magnetic_field.o ../interp2.o ../readfile.o ../vector.o

all: ctsv diff domain magnetic_field input ode

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

../%.o: ../%.c
	$(CC) $(CFLAGS) -o $@ $<

ctsv: $(OCTSV)
	$(LD) $(OCTSV) $(LFLAGS) -o $@ 

diff: $(ODIFF)
	$(LD) $(ODIFF) $(LFLAGS) -o $@

domain: $(ODOMAIN)
	$(LD) $(ODOMAIN) $(LFLAGS) -o $@ 

magnetic_field: $(OMAGFIELD)
	$(LD) $(OMAGFIELD) $(LFLAGS) -o $@ 

input: $(OINPUT)
	$(LD) $(OINPUT) $(LFLAGS) -o $@

ode: $(OODE)
	$(LD) $(OODE) $(LFLAGS) -o $@

fieldFineGrid: $(OFFG)
	$(LD) $(OFFG) $(LFLAGS) -o $@

clean:
	rm *.o
	rm domain

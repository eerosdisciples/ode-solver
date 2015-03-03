# ODE Solver Makefile

CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -c -I include
LD=gcc

LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas 

TARGET=solver
OBJECTS=arguments.o ctsv.o domain.o equation.o magnetic_field.o main.o ode.o readfile.o vector.o interp2.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LD) $(LFLAGS) -o $@ $(OBJECTS)

test:
	make -C tests

clean:
	rm *.o $(UTILS)




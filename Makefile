# ODE Solver Makefile

CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -c -I include -ggdb
LD=gcc

LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas -lm
TARGET=solver
OBJECTS=arguments.o ctsv.o domain.o equation.o interp2.o main.o magnetic_field.o ode.o readfile.o vector.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) $(LFLAGS) -o $@

test:
	make -C tests

clean:
	rm *.o $(UTILS)




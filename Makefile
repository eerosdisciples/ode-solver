# ODE Solver Makefile

CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -c -I include
LD=gcc
<<<<<<< HEAD
LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas
=======
LFLAGS=-lm
>>>>>>> 5b11f0b167d58f216309dd386540df9b12865df7

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


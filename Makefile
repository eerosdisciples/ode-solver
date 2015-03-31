# ODE Solver Makefile

CC=gcc
# Flags for debuggable program
CFLAGS=-Wall -Wextra -g -pg -O0 -c -I include
# Flags for optimized program
#CFLAGS=-Wall -Wextra -O2 -c -I include
LD=gcc

LFLAGS=/usr/local/lib/libinterp2d.a -lgsl -lgslcblas -lm
TARGET=solver
OBJECTS=arguments.o ctsv.o diff.o domain.o equation_GCM.o equation_predprey.o equation_particle.o input.o interp2.o main.o magnetic_field.o quantities.o rkf45.o readfile.o vector.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) $(LFLAGS) -o $@

test:
	make -C tests

clean:
	rm *.o $(UTILS)




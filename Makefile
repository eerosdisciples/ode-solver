# ODE Solver Makefile

CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -c -I include
LD=gcc

TARGET=solver
OBJECTS=arguments.o ctsv.o equation.o main.o ode.o readfile.o vector.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LD) $(OBJECTS) $(LFLAGS) -o $@

test:
	make -C tests

clean:
	rm *.o $(UTILS)


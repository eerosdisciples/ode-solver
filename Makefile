# ODE Solver Makefile

CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -c -I include
LD=gcc
LFLAGS=

TARGET=solver
OBJECTS=magnetic_field.o main.o vector.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LD) $(LFLAGS) -o $@ $(OBJECTS)

clean:
	rm *.o $(UTILS)


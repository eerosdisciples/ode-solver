# ODE Solver Makefile

CC=gcc
CFLAGS=-Wall -Wextra -g -O0 -c
LD=gcc
LFLAGS=

TARGET=solver
OBJECTS=main.o matrix.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET): $(OBJECTS)
	$(LD) $(LFLAGS) -o $@ $(OBJECTS)

clean:
	rm *.o $(UTILS)


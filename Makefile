target := stellarton-system
objs := main.o backorder.o customers.o date.o products.o reorder.o sales.o suppliers.o

CC = gcc
CFLAGS = -c -Wall

all: $(target)

stellarton-system: $(objs)
	$(CC) $^ -O2 -o $@

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	$(RM) *.o
	$(RM) stellarton-system
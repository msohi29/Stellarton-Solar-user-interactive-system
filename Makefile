SRC = src
OBJ = obj
BINDIR = bin

BIN = $(BINDIR)/stellarton-system

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

CC = gcc
CFLAGS = -g -Wall

all: $(BIN)

release: CFLAGS = -Wall -O2 -DNDEBG
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

wclean:
	del /Q /S .\$(BINDIR)\*.exe .\$(OBJ)\*.o *.txt 

clean:
	$(RM) $(BIN) $(OBJS) *.txt
NAME = avenue

SDIR = src
ODIR = build
INC = -Iincludes
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
OUT = bin/$(NAME)
ARGS = testfile.txt

_SRCS = main.c terminal.c abuf.c editor.c
SRCS = $(patsubst %, $(SDIR)/%, $(_SRCS))

_OBJS = main.o terminal.o abuf.o editor.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

$(NAME): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

$(ODIR)/%.o: $(SDIR)/%.c 
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm $(OBJS) $(OUT)
	clear

run:
	./bin/avenue $(ARGS)

runmem:
	valgrind ./bin/avenue $(ARGS)
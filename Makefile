NAME = avenue

SDIR = src
ODIR = build
INC = -Iincludes
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
OUT = bin/$(NAME)

_SRCS = main.c
SRCS = $(patsubst %, $(SDIR)/%, $(_SRCS))

_OBJS = main.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

$(NAME): $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $(SRCS) -o $(OBJS)

clean:
	rm $(OBJS) $(OUT)
	clear

run:
	./bin/avenue

runmem:
	valgrind ./bin/avenue
CC = gcc
SRC = $(wilcard *.c)
OBJ = $(SRC :.c = .o)

all : sort

Tree.o : Tree.c
	$(CC) -c $< -o $@
	
sort : sort.o Tree.o 
	$(CC) $^ -o $@
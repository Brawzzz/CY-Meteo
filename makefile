CC = gcc
SRC = $(wilcard *.c)
OBJ = $(SRC :.c = .o)

all : sort

tools.o : tools.c
	$(CC) -c $< -o $@

Data_Set.o : Data_Set.c
	$(CC) -c $< -o $@

Tree.o : Tree.c
	$(CC) -c $< -o $@
	
sort.o : sort.c
	$(CC) -c $< -o $@

sort : tools.o Data_Set.o Tree.o sort.o 
	$(CC) $^ -o $@
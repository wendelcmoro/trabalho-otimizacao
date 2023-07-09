FLAGS=-O3

CC=gcc

RM=rm -f

EXEC=machines

all: $(EXEC)

$(EXEC):
	$(CC) $(FLAGS) machines.c -c -o machines.o
	$(CC) $(FLAGS) machines.o -o $(EXEC)

run:
	./$(EXEC)

clean:
	$(RM) machines.o $(EXEC)
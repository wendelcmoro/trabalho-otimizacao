FLAGS=-O3

CC=gcc

RM=rm -f

EXEC=caminho

all: $(EXEC)

$(EXEC):
	$(CC) $(FLAGS) caminho.c -c -o caminho.o
	$(CC) $(FLAGS) caminho.o -o $(EXEC)

run:
	./$(EXEC)

clean:
	$(RM) caminho.o $(EXEC)
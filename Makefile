CC=clang
PROG=term
OBJS=main.o logging.o signals.o find.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -ggdb -o $(PROG) $(OBJS)

clean:
	rm -f $(PROG) $(OBJS)

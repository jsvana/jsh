CC=clang
PROG=jsh
OBJS=main.o logging.o signals.o find.o command.o environment.o
CFLAGS=-g -ggdb

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -g -ggdb -o $(PROG) $(OBJS)

clean:
	rm -f $(PROG) $(OBJS)

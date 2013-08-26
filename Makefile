CC=clang
PROG=jsh
OBJS=main.o signals.o find.o command.o environment.o
CFLAGS=-g -ggdb
LDFLAGS=-lreadline

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(CFLAGS) $(LDFLAGS) $(OBJS)

clean:
	rm -f $(PROG) $(OBJS)

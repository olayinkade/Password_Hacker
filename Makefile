CC = clang
CFLAGS = -lpthread -lcrypt -g

PROG = Hacker
HDRS = hash_test.h
SRCS = Hacker.c hash_test.c

OBJDIR = object
OBJS = $(OBJDIR)/Hacker.o $(OBJDIR)/hash_test.o

# compiling rules

# WARNING: *must* have a tab before each definition
$(PROG): $(OBJS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

$(OBJDIR)/hash_test.o: hash_test.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c hash_test.c -o $(OBJDIR)/hash_test.o

$(OBJDIR)/Hacker.o: Hacker.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c Hacker.c -o $(OBJDIR)/Hacker.o

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f $(PROG) $(OBJS)
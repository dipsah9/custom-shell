CC = gcc
CFLAGS = -Wall -Iincludes
LDFLAGS = -lreadline
OBJDIR = obj
SRCDIR = src

OBJS = $(OBJDIR)/main.o $(OBJDIR)/commands.o
EXEC = custom_shell

all: $(EXEC)

$(EXEC): $(OBJS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXEC)

.PHONY: all clean

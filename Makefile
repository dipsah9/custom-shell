CC = gcc
CFLAGS = -Wall -Iincludes
OBJDIR = obj
SRCDIR = src

OBJS = $(OBJDIR)/main.o $(OBJDIR)/commands.o
EXEC = custom_shell

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(EXEC)

.PHONY: all clean

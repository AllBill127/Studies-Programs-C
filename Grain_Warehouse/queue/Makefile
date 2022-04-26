IDIR = .\

CC = gcc

CFLAGS = -I $(IDIR)

DEPS = queue.h queueStruct.h

OBJ = test.o queue.o


%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


IDIR = .\

CC = gcc

CFLAGS = -I $(IDIR)

DEPS = stack.h stackStruct.h

OBJ = test.o stack.o


%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


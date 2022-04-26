IDIR = .\

CC = gcc

CFLAGS = -I $(IDIR)

DEPS = queue.h queueStruct.h stack.h stackStructs.h

OBJ = Grain_Warehouse.o queue.o stack.o


%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


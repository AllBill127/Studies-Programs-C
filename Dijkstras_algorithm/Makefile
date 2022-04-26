IDIR = .

CC = gcc

CFLAGS = -I $(IDIR)

DEPS = min_heap.h adjacency_list.h

OBJ = test.o min_heaps.o adjacency_list.o


%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


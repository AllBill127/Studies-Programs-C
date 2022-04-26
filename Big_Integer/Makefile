IDIR = .

CC = gcc

CFLAGS = -I $(IDIR)

DEPS = big_int.h

OBJ = test.o big_int.o


%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


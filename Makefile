CC = gcc
CFLAGS = -Wall -g
OBJ = main.o btree.o queue.o

btree_app: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

main.o: main.c btree.h
btree.o: btree.c btree.h queue.h
queue.o: queue.c queue.h

clean:
	rm -f *.o btree_app

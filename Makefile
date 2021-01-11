CC = gcc

CFLAGS = -Wall -Werror -fsanitize=address,undefined -g

# The first target is used if no argument is given to make.
memgrind: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) memgrind.o mymalloc.o -o memgrind
	
memgrind.o: mymalloc.h
mymalloc.o: mymalloc.h
clean:
	rm -f project *.o
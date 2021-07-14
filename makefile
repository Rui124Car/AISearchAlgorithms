CFLAGS = -g -Wall

all: ret

ret_bfs: ret.o
	gcc -Wall ret.o -o ret

ret.o: ret.c
	gcc -c $(CFLAGS) ret.c

clean:
	rm *.o ret

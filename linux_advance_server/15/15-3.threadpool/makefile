OBJS=main.o http_conn.o
CC=g++
CFLAGS+=-c -Wall -g -lrt

main:$(OBJS)
	$(CC) $^ -g -o $@ -lrt

%.o:%.cpp
	$(CC) $^ -c -g -o $@

clean:
	$(RM) *.o  main -r

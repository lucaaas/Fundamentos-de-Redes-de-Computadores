all: server client clean

server: server.o math_functions.o socket_configuration.o
	gcc $(CFLAGS) -o server server.o math_functions.o socket_configuration.o

server.o: server.c math_functions.h socket_configuration.h
	gcc $(CFLAGS) -c server.c

math_functions.o: math_functions.c
	gcc $(CFLAGS) -c math_functions.c

socket_configuration.o: socket_configuration.c
	gcc $(CFLAGS) -c socket_configuration.c

client: client.o
	gcc $(CFLAGS) -o client client.o

client.o: client.c
	gcc $(CFLAGS) -c client.c

clean:
	rm -f *.o

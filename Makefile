CC = gcc
CFLAGS = -Wall -Iinclude
OBJ = build/main.o build/flight_operations.o build/serialization.o build/socket_server.o build/utils.o
EXEC = flight_reservation_system

all: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/main.o

build/flight_operations.o: src/flight_operations.c
	$(CC) $(CFLAGS) -c src/flight_operations.c -o build/flight_operations.o

build/serialization.o: src/serialization.c
	$(CC) $(CFLAGS) -c src/serialization.c -o build/serialization.o

build/socket_server.o: src/socket_server.c
	$(CC) $(CFLAGS) -c src/socket_server.c -o build/socket_server.o

build/utils.o: src/utils.c
	$(CC) $(CFLAGS) -c src/utils.c -o build/utils.o

clean:
	rm -f build/*.o $(EXEC)

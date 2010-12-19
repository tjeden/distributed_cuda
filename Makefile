# Makefile for the socket programming example
#

simple_client_objects = ClientSocket.o ServerSocket.o Socket.o simple_client_main.o

all : simple_client
simple_client: $(simple_client_objects)
	g++ -o simple_client $(simple_client_objects)


Socket: Socket.cpp
ClientSocket: ClientSocket.cpp
ServerSocket: ServerSocket.cpp
simple_client_main: simple_client_main.cpp

clean:
	rm -f *.o simple_client

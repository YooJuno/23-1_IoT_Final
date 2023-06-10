CC := g++
CFLAGS := -std=c++11 -Wall -Wextra
LIBS := -lboost_system -lpthread -lboost_thread $(pkg-config --libs --cflags opencv4)
 
all: server client

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp $(LIBS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LIBS)

clean:
	rm -f server client

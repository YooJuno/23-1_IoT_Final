g++ -std=c++11 Client.cpp -o Client -lboost_system -pthread $(pkg-config --libs --cflags opencv4)
g++ -std=c++11 Server.cpp -o Server -lboost_system -pthread
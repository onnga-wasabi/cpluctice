CC := g++
CFLAGS := -std=c++11
HTTP1 = server
HTTP1_SOURCE = server.cpp socket.cpp parser.cpp response.cpp

$(HTTP1): $(HTTP1_SOURCE)
	g++ $(HTTP1_SOURCE) -o $(HTTP1)

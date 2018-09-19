client: menu.o
	g++ menu.o -lcrypto -o client

menu.o: menu.cpp mtorrent_handle.h tracker_socket.h
	g++ -std=c++11 -c menu.cpp

clean:
	rm *.o client

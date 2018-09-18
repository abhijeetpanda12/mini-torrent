client: menu.o
	g++ menu.o -lcrypto -o client

menu.o: menu.cpp mtorrent_handle.h tracker_socket.h
	g++ -c menu.cpp

clean:
	rm *.o client

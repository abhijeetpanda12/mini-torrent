tracker: tracker.o
	g++ tracker.o -o tracker

tracker.o: tracker.cpp tracker_util.h request_util.h
	g++ -std=c++11 -c tracker.cpp

clean:
	rm *.o tracker

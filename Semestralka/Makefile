all: main.o CServer.o CConfig.o CLogger.o CUtils.o
	g++ -Wall -pedantic main.o CServer.o CConfig.o CLogger.o CUtils.o

clean:
	rm -f a.out *.o

main.o: src/main.cpp src/CServer.h
	g++ -Wall -pedantic -c src/main.cpp -o main.o

CServer.o: src/CServer.cpp src/CServer.h src/CLogger.h src/CConfig.h src/CUtils.h
	g++ -Wall -pedantic -c src/CServer.cpp -o CServer.o

CLogger.o: src/CLogger.h src/CLogger.cpp
	g++ -Wall -pedantic -c src/CLogger.cpp -o CLogger.o

CConfig.o: src/CConfig.cpp src/CConfig.h libs/json.hpp
	g++ -Wall -pedantic -c src/CConfig.cpp -o CConfig.o

CUtils.o: src/CUtils.h src/CUtils.cpp
	g++ -Wall -pedantic -c src/CUtils.cpp -o CUtils.o

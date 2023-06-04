all: main.o CServer.o CConfig.o CLogger.o util.o CHTTPMethods.o CGetFormats.o CGet.o CPost.o
	g++ -Wall -pedantic main.o CServer.o CConfig.o CLogger.o util.o CHTTPMethods.o CGetFormats.o CGet.o CPost.o

clean:
	rm -f a.out *.o

main.o: src/main.cpp src/CServer.h src/CLogger.h src/CConfig.h
	g++ -Wall -pedantic -c src/main.cpp -o main.o

CServer.o: src/CServer.cpp src/CServer.h src/CLogger.h src/CConfig.h src/util.h src/CHTTPMethods.h src/CGet.h src/CPost.h
	g++ -Wall -pedantic -c src/CServer.cpp -o CServer.o

CHTTPMethods.o: src/CHTTPMethods.h src/CHTTPMethods.cpp src/util.h src/CConfig.h src/CLogger.h
	g++ -Wall -pedantic -c src/CHTTPMethods.cpp -o CHTTPMethods.o

CGet.o: src/CGet.cpp src/CGet.h src/CConfig.h src/util.h src/CLogger.h src/CGetFormats.h
	g++ -Wall -pedantic -c src/CGet.cpp -o CGet.o

CPost.o: src/CPost.cpp src/CPost.h src/CConfig.h src/util.h src/CLogger.h src/CGetFormats.h
	g++ -Wall -pedantic -c src/CPost.cpp -o CPost.o

CGetFormats.o: src/CGetFormats.h src/CGetFormats.cpp src/util.h
	g++ -Wall -pedantic -c src/CGetFormats.cpp -o CGetFormats.o

CLogger.o: src/CLogger.h src/CLogger.cpp src/CConfig.h
	g++ -Wall -pedantic -c src/CLogger.cpp -o CLogger.o

util.o: src/util.h src/util.cpp src/CConfig.h
	g++ -Wall -pedantic -c src/util.cpp -o util.o

CConfig.o: src/CConfig.cpp src/CConfig.h libs/json.hpp
	g++ -Wall -pedantic -c src/CConfig.cpp -o CConfig.o
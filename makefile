all: main.cpp ConfigFile.o MetaData.o Utils.o Logger.o
	g++ -g main.cpp ConfigFile.o MetaData.o Utils.o Logger.o -o main

ConfigFile.o: ConfigFile.cpp ConfigFile.h
	g++ -c ConfigFile.cpp

MetaData.o: MetaData.cpp MetaData.h
	g++ -c MetaData.cpp

Utils.o: Utils.cpp Utils.h
	g++ -c Utils.cpp

Logger.o: Logger.cpp Logger.h
	g++ -c Logger.cpp

clean:
	rm *.o main

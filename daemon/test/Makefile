# NOTE: To ascertain include directories and lib specifications, run...
# $pkg-config --cflags --libs libmongoc-1.0 
#
LIBS = -L/usr/local/lib -lmongoc-1.0 -lssl -lcrypto -lrt -lbson-1.0
CFLAGS = -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 
DEBUG=-ggdb
#OPTS=-O2
OPTS=
WARNINGS=-Wall -Werror
CC=g++

all: build/example-client

build/example-client: build/example-client.o
	g++ $(DEBUG) $(WARNINGS) $(OPTS) -o build/example-client build/example-client.o $(LIBS)

build/example-client.o: example-client.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) -o build/example-client.o $(CFLAGS) example-client.cpp

clean:
	rm build/example-client.o build/example-client

# NOTE: To ascertain include directories and lib specifications, run...
# $pkg-config --cflags --libs libmongoc-1.0 
all: build/example-client

build/example-client: build/example-client.o
	g++ -g -o build/example-client build/example-client.o -L/usr/local/lib -lmongoc-1.0 -lssl -lcrypto -lrt -lbson-1.0 

build/example-client.o: example-client.cpp
	g++ -c -o build/example-client.o -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 example-client.cpp

clean:
	rm build/example-client.o build/example-client

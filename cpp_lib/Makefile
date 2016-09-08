# NOTE: To ascertain include directories and lib specifications, run...
# ...for MongoDB: 
# $pkg-config --cflags libmongoc-1.0 
# $pkg-config --libs libmongoc-1.0 
#MONGO_CFLAGS = -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 
#MONGO_LIBS = -L/usr/local/lib -lmongoc-1.0 -lssl -lcrypto -lrt -lbson-1.0
# ...for curl:
# $curl-config --cflags
# $curl-config --libs
# CURL_LIBS = -L/usr/local/lib -lcurl
# CURL_CFLAGS = -I/usr/local/include

CPP_LIB = .

CFLAGS = -I/usr/local/include -I$(CPP_LIB) -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 
# For shared_ptr, -std=c++0x
CFLAGS += -std=c++0x
# If you want C++11...
# http://stackoverflow.com/questions/36816570/glibcxx-3-4-21-not-defined-in-file-libstdc-so-6-with-link-time-reference
# Need -D_GLIBCXX_USE_CXX11_ABI=0 if you have an older "ABI"...consider
# simply updgrading ubuntu and it will come with a C++11 g++ installed,
# perhaps...
# symbol _xxx_ version GLIBCXX_3.4.21 not defined in file libstdc++. so.6 with link time reference
#CFLAGS += -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
LIBS = -L/usr/local/lib -lcurl -L/usr/local/lib -lmongoc-1.0 -lssl -lcrypto -lrt -lbson-1.0

DEBUG=-ggdb

# Beware, -O2 optimizes variables out of gdb debugger...
#OPTS=-O2
OPTS=
#WARNINGS=-Wall -Werror -Wno-reorder 
#WARNINGS=-Wall -Wno-reorder -fpermissive -Wunused-variable 
#WARNINGS=-Wall -Wno-reorder -fpermissive 
WARNINGS=-Wall -Wno-reorder
#CC=g++ -v
CC=g++
#CC=/usr/bin/g++

all: build/ftpget build/mongoc

build/ftpget: build/ftpget.o build/utils.o build/FtpClient.o
	$(CC) $(DEBUG) $(WARNINGS) $(OPTS) -o build/ftpget build/ftpget.o build/utils.o build/FtpClient.o $(LIBS) 

build/mongoc: build/mongoc.o build/utils.o build/MongoDbClient.o build/BsonTraverser.o build/BsonPrettyPrintTraverser.o
	$(CC) $(DEBUG) $(WARNINGS) $(OPTS) -o build/mongoc build/mongoc.o build/utils.o build/MongoDbClient.o build/BsonTraverser.o build/BsonPrettyPrintTraverser.o $(LIBS) 

build/ftpget.o: ftpget.cpp
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/ftpget.o ftpget.cpp

build/mongoc.o: mongoc.cpp
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/mongoc.o mongoc.cpp

build/FtpClient.o: ./FtpClient.cpp ./FtpClient.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FtpClient.o ./FtpClient.cpp

build/MongoDbClient.o: ./MongoDbClient.cpp ./MongoDbClient.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/MongoDbClient.o ./MongoDbClient.cpp

build/BsonTraverser.o: ./BsonTraverser.cpp ./BsonTraverser.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/BsonTraverser.o ./BsonTraverser.cpp

build/BsonPrettyPrintTraverser.o: ./BsonPrettyPrintTraverser.cpp ./BsonPrettyPrintTraverser.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/BsonPrettyPrintTraverser.o ./BsonPrettyPrintTraverser.cpp

build/utils.o: ./utils.cpp
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/utils.o ./utils.cpp

clean:
	rm build/*.o build/ftpget build/mongoc

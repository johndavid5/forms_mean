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
CPP_LIB = ../cpp_lib

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

all: build/sec_forms_daemon

OBJECTS = build/sec_forms_daemon.o build/Forms.o build/EdgarForm.o build/FormsUtils.o build/FormsCommon.o build/BsonFormParseTraverser.o build/utils.o build/FtpClient.o build/MongoDbClient.o build/BsonPrettyPrintTraverser.o build/BsonTraverser.o

build/sec_forms_daemon: $(OBJECTS)
	$(CC) $(DEBUG) $(WARNINGS) $(OPTS) -o build/sec_forms_daemon $(OBJECTS) $(LIBS) 

build/sec_forms_daemon.o: sec_forms_daemon.cpp
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/sec_forms_daemon.o sec_forms_daemon.cpp

build/Forms.o: Forms.cpp Forms.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/Forms.o Forms.cpp

build/EdgarForm.o: EdgarForm.cpp EdgarForm.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/EdgarForm.o EdgarForm.cpp

build/FormsUtils.o: FormsUtils.cpp FormsUtils.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FormsUtils.o FormsUtils.cpp

build/FormsCommon.o: FormsCommon.cpp FormsCommon.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FormsCommon.o FormsCommon.cpp

build/BsonFormParseTraverser.o: BsonFormParseTraverser.cpp BsonFormParseTraverser.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/BsonFormParseTraverser.o BsonFormParseTraverser.cpp

build/FtpClient.o: $(CPP_LIB)/FtpClient.cpp $(CPP_LIB)/FtpClient.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FtpClient.o $(CPP_LIB)/FtpClient.cpp

build/MongoDbClient.o: $(CPP_LIB)/MongoDbClient.cpp $(CPP_LIB)/MongoDbClient.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/MongoDbClient.o $(CPP_LIB)/MongoDbClient.cpp

build/BsonTraverser.o: $(CPP_LIB)/BsonTraverser.cpp $(CPP_LIB)/BsonTraverser.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/BsonTraverser.o $(CPP_LIB)/BsonTraverser.cpp

build/BsonPrettyPrintTraverser.o: $(CPP_LIB)/BsonPrettyPrintTraverser.cpp $(CPP_LIB)/BsonPrettyPrintTraverser.h
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/BsonPrettyPrintTraverser.o $(CPP_LIB)/BsonPrettyPrintTraverser.cpp

build/utils.o: $(CPP_LIB)/utils.cpp
	$(CC) -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/utils.o $(CPP_LIB)/utils.cpp

clean:
	rm build/*.o build/sec_forms_daemon

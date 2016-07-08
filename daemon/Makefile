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

CFLAGS = -I/usr/local/include -I./cpp_lib -I/usr/local/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 
# For shared_ptr, -std=c++0x
CFLAGS += -std=c++0x
# If you want C++11...
# CFLAGS += -std=c++11
LIBS = -L/usr/local/lib -lcurl -L/usr/local/lib -lmongoc-1.0 -lssl -lcrypto -lrt -lbson-1.0

DEBUG=-ggdb

# Beware, -O2 optimizes variables out of gdb debugger...
#OPTS=-O2
OPTS=
#WARNINGS=-Wall -Werror -Wno-reorder 
#WARNINGS=-Wall -Wno-reorder -fpermissive -Wunused-variable 
#WARNINGS=-Wall -Wno-reorder -fpermissive 
WARNINGS=-Wall -Wno-reorder
CC=g++ -v

all: build/ftpget build/mongoc build/forms_mean_daemon

build/forms_mean_daemon: build/forms_mean_daemon.o build/Forms.o build/EdgarForm.o build/FormsMeanUtils.o build/FormsMeanCommon.o build/utils.o build/FtpClient.o build/MongoDbClient.o
	g++ $(DEBUG) $(WARNINGS) $(OPTS) -o build/forms_mean_daemon build/forms_mean_daemon.o build/Forms.o build/EdgarForm.o build/FormsMeanUtils.o build/FormsMeanCommon.o build/utils.o build/FtpClient.o build/MongoDbClient.o $(LIBS) 

build/ftpget: build/ftpget.o build/utils.o build/FtpClient.o
	g++ $(DEBUG) $(WARNINGS) $(OPTS) -o build/ftpget build/ftpget.o build/utils.o build/FtpClient.o $(LIBS) 

build/mongoc: build/mongoc.o build/utils.o build/MongoDbClient.o
	g++ $(DEBUG) $(WARNINGS) $(OPTS) -o build/mongoc build/mongoc.o build/utils.o build/MongoDbClient.o $(LIBS) 

build/ftpget.o: ftpget.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/ftpget.o ftpget.cpp

build/mongoc.o: mongoc.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/mongoc.o mongoc.cpp

build/forms_mean_daemon.o: forms_mean_daemon.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/forms_mean_daemon.o forms_mean_daemon.cpp

build/Forms.o: Forms.cpp Forms.h
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/Forms.o Forms.cpp

build/EdgarForm.o: EdgarForm.cpp EdgarForm.h
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/EdgarForm.o EdgarForm.cpp

build/FormsMeanUtils.o: FormsMeanUtils.cpp FormsMeanUtils.h
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FormsMeanUtils.o FormsMeanUtils.cpp

build/FormsMeanCommon.o: FormsMeanCommon.cpp FormsMeanCommon.h
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FormsMeanCommon.o FormsMeanCommon.cpp

build/FtpClient.o: ./cpp_lib/FtpClient.cpp ./cpp_lib/FtpClient.h
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/FtpClient.o ./cpp_lib/FtpClient.cpp

build/MongoDbClient.o: ./cpp_lib/MongoDbClient.cpp ./cpp_lib/MongoDbClient.h
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/MongoDbClient.o ./cpp_lib/MongoDbClient.cpp

build/utils.o: ./cpp_lib/utils.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/utils.o ./cpp_lib/utils.cpp

clean:
	rm build/*.o build/ftpget build/mongoc build/forms_mean_daemon

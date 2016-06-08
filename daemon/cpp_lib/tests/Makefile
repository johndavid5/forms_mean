CFLAGS = -I..
LIBS = -L/usr/local/lib -lcurl

DEBUG=-ggdb

# Beware, -O2 optimizes variables out of gdb debugger...
#OPTS=-O2
OPTS=
#WARNINGS=-Wall -Werror -Wno-reorder 
#WARNINGS=-Wall -Wno-reorder -fpermissive -Wunused-variable 
WARNINGS=-Wall -Wno-reorder -fpermissive 
CC=g++

all: build/logger_demo

build/logger_demo: build/logger_demo.o build/utils.o
	g++ $(DEBUG) $(WARNINGS) $(OPTS) -o build/logger_demo build/logger_demo.o build/utils.o $(LIBS) 

build/logger_demo.o: logger_demo.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/logger_demo.o logger_demo.cpp

build/utils.o: ../utils.cpp
	g++ -c $(DEBUG) $(WARNINGS) $(OPTS) $(CFLAGS) -o build/utils.o ../utils.cpp

clean:
	rm build/logger_demo.o build/logger_demo build/utils.o

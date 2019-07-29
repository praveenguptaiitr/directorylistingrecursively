LIBS = -lm
CC = gcc
CFLAGS = -g -pthread 
SOURCES = dirlist.c

TARGET = dirlist

default: all

all: clean $(TARGET) 

$(TARGET):
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

clean:
	rm -rf $(TARGET)
	

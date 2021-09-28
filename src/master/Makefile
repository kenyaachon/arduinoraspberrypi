# ----------------------------------------------------------------------
#	Makefile for client-server application
#	Use: make
# ----------------------------------------------------------------------

CC = g++
CFLAGS = -g -Wall -pthread
LDFLAGS   = -pthread 
LOADLIBES = -lrt -lm

SOURCES = master.cpp
TARGET = master

#OPENCV = `pkg-config opencv --cflags --libs`
OPENCV = `pkg-config --cflags --libs ~/opencv/build/OpenCV-3.4.9/lib/pkgconfig/opencv.pc`
LIBS = $(OPENCV)

$(TARGET):$(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SOURCES) $(LOADLIBES) $(LIBS)

.PHONY: clean
clean : 
	@rm -f core *.o *.out *.bb *.bbg *.gcov *.da *~
	@rm -f $(TARGET)


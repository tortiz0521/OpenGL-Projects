.PHONY: all clean

all: hello

CXX = g++
objects = hello.o func.o

hello: $(objects)
	$(CXX) -o hello $(objects)

func.o: func.cpp func.h
	g++ -c func.cpp

hello.o: hello.cpp
	g++ -c hello.cpp

# Clean up the file!
clean:
	rm *.o hello